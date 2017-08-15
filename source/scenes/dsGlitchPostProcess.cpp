
#include "dsGlitchPostProcess.h"

static bool compareTLItem(const Math::TimelineItem<DS::Stage*> & a, const Math::TimelineItem<DS::Stage*> & b) {
    return a.GetObject()->GetOrder() < b.GetObject()->GetOrder();
}


dsGlitchPostProcess::dsGlitchPostProcess(DS::Data * data) : DS::Stage(data) {
    fboManager = NULL;
    fboTexture = NULL;
    aaProgram = NULL;
}

dsGlitchPostProcess::~dsGlitchPostProcess() {
    SafeDelete(fboManager);
    SafeDelete(fboTexture);
    SafeDelete(aaProgram);
}

void dsGlitchPostProcess::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    fboManager = dev->CreateRTManager(m_Data->GetWidth(), m_Data->GetHeight());
    fboTexture = dynamic_cast<Graph::Texture2D*>(
        dev->CreateTexture(Graph::TEXTURE_2D, m_Data->GetWidth(), m_Data->GetHeight(), 0, Graph::FORMAT_RGBA_8B, true)
        );
    fboManager->Attach(0, fboTexture);

    aaProgram = m_Data->LoadProgram("shader://glitch.cpp");
    noiseTexture = dynamic_cast<Graph::Texture2D*>(m_Data->LoadTexture("texture://tex2d_noise.png"));
}

void dsGlitchPostProcess::Update(int64_t start, int64_t end, int64_t time) {
    std::list<Math::TimelineItem<DS::Stage*>> items;
    m_Stages.Get(time, &items);
    items.sort(compareTLItem);
    ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
        i->GetObject()->Update(i->GetStart(), i->GetEnd(), time);
    }
}

void dsGlitchPostProcess::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    std::list<Math::TimelineItem<DS::Stage*>> items;
    m_Stages.Get(time, &items);
    items.sort(compareTLItem);
    
    ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
        i->GetObject()->RenderFBO(i->GetStart(), i->GetEnd(), time);
    }

    fboManager->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, fboTexture->GetWidth(), fboTexture->GetHeight());

        ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
            i->GetObject()->Render(i->GetStart(), i->GetEnd(), time);
        }
    }
    fboManager->Disable();
}


void dsGlitchPostProcess::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;

   // dev->Viewport(0,0, width, height);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Disable(Graph::STATE_DEPTH_TEST);
    //dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_CULL_FACE);

    fboTexture->Enable(0);
    noiseTexture->Enable(1);

    aaProgram->Enable();
    aaProgram->SetVariable4f("texResolution", width, height);
    aaProgram->SetVariable1f("time", t);

    dev->PushMatrix();
    dev->Scale(1, -1, 0);
    dev->Translate(0, -height, 0);
    shp->Square(0, 0, width, height, Math::Color4ub());
    dev->PopMatrix();

    aaProgram->Disable();

    noiseTexture->Disable(1);
    fboTexture->Disable(0);
}

