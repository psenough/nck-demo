
#include "nckWindow.h"
#include "nckThread.h"
#include "nckGraphics.h"
#include "nckVec3.h"
#include "nckMat44.h"
#include "nckException.h"
#include "nckUtils.h"
#include "nckTimeline.h"
#include "nckFont.h"
#include "nckColor4.h"
#include "nckMathUtils.h"
#include "nckVec2.h"
#include "nckWidget.h"
#include "nckAudioDevice.h"
#include "nckQueueBuffer.h"
#include "nckGeometry.h"
#include "nckModel.h"

#include "dConfig.h"
#include "dUtils.h"
#include "dSettings.h"
#include "dSubtitles.h"

class Canvas {
public:
    Canvas(float w, float h) {
        width = w;
        height = h;
    }
    float width;
    float height;
};

float sign(float v) {
    if (v < 0.0)
        return -1.0;
    else if (v > 0.0)
        return 1.0;
    else
        return 0.0;
}

class Particle {
public:
    Particle(Canvas canvas) {
        x = Math::RandomValue() * canvas.width;
        y = Math::RandomValue() * canvas.height;
        life = Math::RandomValue() * 4000 + 2000;
        i_life = 1.0 / life;

        vx = Math::RandomValue() * 2.0 - 1.0;
        vy = Math::RandomValue() * 2.0 - 1.0;

        i_size = Math::RandomValue() * 10.0 + 45.0;
    }

    void update(float dt, Canvas canvas) {
        float v = life * i_life;

        life -= dt;
        size = v * i_size;

        v *= v;

        float cx = (x - canvas.width * 0.5) / canvas.height;
        float cy = (y - canvas.height * 0.5) / canvas.height;

        y += vy * dt * v * 0.125 + cy * cy * sign(cy) * 4.0;
        x += vx * dt * v * 0.125 + cx * cx * sign(cx) * 4.0;
    }

   void render(Gui::ShapeRenderer * renderer) {
       float a = 1.0 - life * i_life;
       if (a < 0.0)
           a = 0.0;
       if (a > 1.0)
           a = 1.0;
       renderer->Square(x - 0.5 * size, y - 0.5 * size, size, size, Math::Color4ub(255, 255, 255, a * 255));
    }

    static void updateParticles(Canvas canvas, std::vector<Particle> * particles, int frame, float dt) {
        int buffer = frame & 1;
        int other_buffer = buffer ^ 1;

        particles[buffer].clear();

        for (int i = 0; i < 10; ++i) {
            Particle p(canvas);
            particles[buffer].push_back(p);
        }
        
        for (int i = 0; i < particles[other_buffer].size();i++) {
            particles[other_buffer][i].update(dt, canvas);
            if (particles[other_buffer][i].life >= 0.0)
                particles[buffer].push_back(particles[other_buffer][i]);
        }
    }

    static void renderParticles(Gui::ShapeRenderer * renderer, int frame, std::vector<Particle> * particles) {
        int buffer = frame & 1;

        for (int i = 0; i < particles[buffer].size(); i++) {
            particles[buffer][i].render(renderer);
        }
    }

    float x, y, life, i_life, size, vy,vx, i_size;
};

class GraphicRendering : public virtual Core::Threadable, public virtual Audio::DeviceCallback
{
public:
    GraphicRendering(Core::Window * window) : Threadable() {
        wnd = window;
        dev = NULL;
        fontMap = NULL;
        fontTexture = NULL;

        sDensity = Core::Window::GetDisplayDensity();
        hScale = wnd->GetHeight() / (float)REFERENCE_HEIGHT;
        decodeBuffer = new uint8_t[1024 * 1024];

        audioBuffer = new Core::QueueBuffer();
        audioStream = NULL;
        audioDevice = NULL;
    }

    ~GraphicRendering() {
        std::list<Subtitle*> subs;
        subtitlesTimeline.GetAll(&subs);
        ListFor(Subtitle*, subs, i) {
            delete (*i);
        }

        SafeDelete(decodeBuffer);
        SafeDelete(audioStream);
        SafeDelete(audioBuffer);
        SafeDelete(audioDevice);
    }

    void AddSub(float start, float duration, const std::string & text) {
        Subtitle * s = subtitlesTimeline.Insert(Math::TimelineItem<Subtitle*>(start, start+duration, new Subtitle(dev, fontTexture, fontMap)));
        s->Set(text, hScale * REFERENCE_SUBTITLE_SIZE);
    }

    void Run() {
       
        Graph::Program * particle = NULL;
        Graph::Program * pCanvas = NULL;
        Graph::RTManager * rtManager = NULL;
        Graph::Texture2D * rtTexture = NULL;

        try {
            dev = Graph::CreateDevice(wnd, Graph::DEVICE_AUTO, wnd->GetWidth(), wnd->GetHeight());
            dev->ClearColor(0.0, 0.0, 0.0, 1.0);
            dev->ClearFlags(Graph::BUFFER_COLOR_BIT | Graph::BUFFER_DEPTH_BIT);

            RenderLoading(wnd, dev);
            
            fontTexture = dev->LoadTexture("texture://tex2d_sans_serif.png");
            fontTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NEAREST);
            fontTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_LINEAR);
            fontTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_LINEAR);

            fontMap = new Gui::FontMap(dev);
            fontMap->Load("script://sans_serif.txt");
            fontMap->SetPositionAccuracy(Gui::FONT_POSITION_ACCURACY_INTEGER);

            particle = dev->LoadProgram("shader://particle.cpp");
            pCanvas = dev->LoadProgram("shader://canvas.cpp");
            pCanvas->SetVariable1i("texture_0", 0);

            shpRender = new Gui::ShapeRenderer(dev);
                        
            rtManager = dev->CreateRTManager(wnd->GetWidth(), wnd->GetHeight());
            rtTexture = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, wnd->GetWidth(), wnd->GetHeight()));

            rtManager->Attach(0, rtTexture);
                        
            audioDevice = dynamic_cast<Audio::OutputDevice*>(Audio::CreateDevice(Audio::DEVICE_OUTPUT));
            audioDevice->SetCallback(this);
            audioStream = audioDevice->LoadStream("audio://jungle01.ogg");
        }
        catch (const Core::Exception & ex) {
            ex.PrintStackTrace();
            SafeDelete(dev);
            return;
        }
               
        AddSub(2, 4, "Demoscene\n2016!");
        AddSub(8, 4, "Scene-pt\nYo yo, toca a levantar a mini!");
        AddSub(14, 4, "E toca a codar uma demo!");
        AddSub(20, 4, "Efeito por\nxernobyl");
        AddSub(26, 4, "Arruinado por\nzeroshift");
        AddSub(32, 4, "Ye");

        std::vector<Particle> particles[2];

        float time = 0.0;
        Core::Chronometer * chron = Core::CreateChronometer();
        chron->Start();

        audioDevice->Play();
           
        float last_dt = 0;
        int frame = 0;

        bool clickReload = false;
        while (!IsTearingDown())
        {
            if (wnd->GetKeyStatus(Core::BUTTON_SPACE) == Core::BUTTON_STATUS_UP && clickReload) {
                try {
                    Graph::Program * tmp1 = dev->LoadProgram("shader://particle.cpp");
                    Graph::Program * tmp2 = dev->LoadProgram("shader://canvas.cpp");
                    SafeDelete(particle);
                    SafeDelete(pCanvas);
                    particle = tmp1;
                    pCanvas = tmp2;
                }
                catch (const Core::Exception & ex) {
                    ex.PrintStackTrace();
                }
                clickReload = false;
            }

            if (wnd->GetKeyStatus(Core::BUTTON_SPACE) == Core::BUTTON_STATUS_DOWN) {
                clickReload = true;
            }

            const float t = chron->GetElapsedTime();
            Canvas canvas(wnd->GetWidth(), wnd->GetHeight());

            float l = (sin(t * 1000) * 0.05 + 1.0) + Math::RandomValue() * 0.0125;
            //l *= 0.1;
            Math::Color4f col = Math::Color4f(l, l * 0.75, l * 0.725, 1.0);

            rtManager->Enable();
            {
                //dev->Clear();
                dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());

                dev->MatrixMode(Graph::MATRIX_PROJECTION);
                dev->Identity();
                dev->Ortho2D(wnd->GetWidth(), wnd->GetHeight());

                dev->MatrixMode(Graph::MATRIX_MODEL);
                dev->Identity();

                dev->Enable(Graph::STATE_BLEND);
                dev->Disable(Graph::STATE_DEPTH_TEST);
                dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

                particle->Enable();
                particle->SetVariable4f("color", col.GetR(), col.GetG(), col.GetB(), col.GetA());
                Particle::renderParticles(shpRender, frame, particles);
                particle->Disable();
            }
            rtManager->Disable();

            dev->Clear();
            dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());

            dev->MatrixMode(Graph::MATRIX_PROJECTION);
            dev->Identity();
            dev->Ortho2D(wnd->GetWidth(), wnd->GetHeight());

            dev->MatrixMode(Graph::MATRIX_MODEL);
            dev->Identity();
                       
            dev->Enable(Graph::STATE_BLEND);
            dev->Disable(Graph::STATE_DEPTH_TEST);
            dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

           
            rtTexture->Enable();
            pCanvas->Enable();
            shpRender->Square(0, 0, wnd->GetWidth(), wnd->GetHeight(), Math::Color4ub());
            pCanvas->Disable();
            rtTexture->Disable();
           

            float tSecs = t / 1000.0;
            std::list<Math::TimelineItem<Subtitle*>> res;
            subtitlesTimeline.Get(tSecs, &res);
            ListFor(Math::TimelineItem<Subtitle*>, res, i) {
                Subtitle * s = (*i).GetObject();
                s->SetPosition(Math::Vec2(wnd->GetWidth() * 0.5, wnd->GetHeight() - hScale * REFERENCE_SUBTITLE_SIZE * 2.0));
                s->Render(tSecs, i->GetStart(), i->GetEnd() - i->GetStart());
            }

            dev->PresentAll();

            float dt = t - last_dt;
            dt *= 5;
            Particle::updateParticles(canvas, particles, frame, dt);

            last_dt = t;
            frame++;

            //Core::Thread::Wait(1000);
        }

        audioDevice->Stop();

        SafeDelete(chron);
        SafeDelete(shpRender);
        SafeDelete(fontTexture);
        SafeDelete(fontMap);
        SafeDelete(dev);
    }

    void UpdateWndInput() {
        if (wnd->GetKeyStatus(Core::BUTTON_ESCAPE) != Core::BUTTON_STATUS_UP)
            Teardown();
    }

    int Buffer(Audio::Device * dev, int bufferSize, uint8_t * bufferData) {
        int dataToRead = dev->GetChannelsCount() * 2 * 4000;
        if (audioBuffer->Size() >= dataToRead)
        {
            return (int)audioBuffer->Pop(bufferData, dataToRead);
        }

        int readSize = audioStream->Read(dataToRead, decodeBuffer);

        if (!readSize) {
            dynamic_cast<Audio::SeekStream*>(audioStream)->SetTime(0);
            readSize = audioStream->Read(dataToRead, decodeBuffer);
        }

        if (!readSize) {
            Core::DebugLog("Error: Unable to restart audio file\n");
        }
        else {
            audioBuffer->Push(decodeBuffer, readSize);
            readSize = (int)audioBuffer->Pop(bufferData, dataToRead);
        }

        return readSize;
    }

private:
    Math::TimelineNode<Subtitle*> subtitlesTimeline;

    uint8_t             * decodeBuffer;
    Audio::Stream       * audioStream;
    Core::QueueBuffer   * audioBuffer;
    Audio::OutputDevice * audioDevice;

    float sDensity;
    float hScale;

    Gui::ShapeRenderer * shpRender;
    Graph::Device   * dev;
    Core::Window    * wnd;
    Graph::Texture	* fontTexture;
    Gui::FontMap    * fontMap;
};

void Core::Application_Main(const std::vector<std::string> & CmdLine)
{
    DemoSettings * conf = new DemoSettings();
    int width = 1024, height = 768;
    bool fullscreen = false;

    bool runDemo = true;// conf->Run(&width, &height, &fullscreen);

    SafeDelete(conf);

    if (runDemo) 
    {
        Core::Window * wnd = Core::CreateWindow("nck-demo", width, height, fullscreen);

        GraphicRendering * grThread = new GraphicRendering(wnd);

        grThread->Start();
        while (wnd->Peek(true)) 
        {
            wnd->SetCursorVisiblity(false);

            if (!grThread->IsRunning())
                break;
            
            grThread->UpdateWndInput();
        }
        grThread->TearDownAndJoin();

        SafeDelete(grThread);
        SafeDelete(wnd);
    }
}

