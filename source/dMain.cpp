
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

#include "dConfig.h"
#include "dUtils.h"
#include "dSettings.h"
#include "dSubtitles.h"

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

    void Run() {
       
        try {
            dev = Graph::CreateDevice(wnd, Graph::DEVICE_AUTO, wnd->GetWidth(), wnd->GetHeight());
            dev->ClearColor(0.0, 0.0, 0.0, 1.0);
            dev->ClearFlags(Graph::BUFFER_COLOR_BIT | Graph::BUFFER_DEPTH_BIT);

            RenderLoading(wnd, dev);

            audioDevice = dynamic_cast<Audio::OutputDevice*>(Audio::CreateDevice(Audio::DEVICE_OUTPUT));
            audioDevice->SetCallback(this);
            audioStream = audioDevice->LoadStream("audio://jungle01.ogg");

            fontTexture = dev->LoadTexture("texture://tex2d_sans_serif.png");
            fontTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NEAREST);
            fontTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_LINEAR);
            fontTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_LINEAR);

            fontMap = new Gui::FontMap(dev);
            fontMap->Load("script://sans_serif.txt");
            fontMap->SetPositionAccuracy(Gui::FONT_POSITION_ACCURACY_INTEGER);

            shpRender = new Gui::ShapeRenderer(dev);
        }
        catch (const Core::Exception & ex) {
            ex.PrintStackTrace();
            SafeDelete(dev);
            return;
        }
               
        Subtitle * s = subtitlesTimeline.Insert(Math::TimelineItem<Subtitle*>(2, 10, new Subtitle(dev, fontTexture, fontMap)));
        s->Set("Demoscene\n2016!", hScale * REFERENCE_SUBTITLE_SIZE);

        s = subtitlesTimeline.Insert(Math::TimelineItem<Subtitle*>(12, 20, new Subtitle(dev, fontTexture, fontMap)));
        s->Set("Scene-pt\nYo yo, toca a levantar a mini!", hScale * REFERENCE_SUBTITLE_SIZE);

        s = subtitlesTimeline.Insert(Math::TimelineItem<Subtitle*>(22, 30, new Subtitle(dev, fontTexture, fontMap)));
        s->Set("E toca a codar uma demo!", hScale * REFERENCE_SUBTITLE_SIZE);

        float time = 0.0;
        Core::Chronometer * chron = Core::CreateChronometer();
        chron->Start();

        audioDevice->Play();
           
        while (!IsTearingDown())
        {
            dev->Clear();
            dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());

            dev->Disable(Graph::STATE_DEPTH_TEST);

            dev->MatrixMode(Graph::MATRIX_PROJECTION);
            dev->Identity();
            dev->Ortho2D(wnd->GetWidth(), wnd->GetHeight());

            dev->MatrixMode(Graph::MATRIX_MODEL);
            dev->Identity();

            float t = chron->GetElapsedTime() / 1000.0;

            dev->Enable(Graph::STATE_BLEND);
            dev->Disable(Graph::STATE_DEPTH_TEST);
            dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

            shpRender->Square(wnd->GetWidth() * 0.5-5.0, wnd->GetHeight() * 0.5-5.0, 10, 10, Math::Color4ub(255, 255, 255));
           
            std::list<Math::TimelineItem<Subtitle*>> res;
            subtitlesTimeline.Get(t, &res);

            ListFor(Math::TimelineItem<Subtitle*>, res, i) {
                Subtitle * s = (*i).GetObject();
                s->SetPosition(Math::Vec2(wnd->GetWidth() * 0.5, wnd->GetHeight() - hScale * REFERENCE_SUBTITLE_SIZE * 2.0));
                s->Render(t, i->GetStart(), i->GetEnd() - i->GetStart());
            }

            dev->PresentAll();
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
    int width = 1280, height = 720;
    bool fullscreen = false;

    bool runDemo =  conf->Run(&width, &height, &fullscreen);

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

