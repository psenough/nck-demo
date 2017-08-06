
#include "dsOutputStage.h"

static bool compareTLItem(const Math::TimelineItem<DS::Stage*> & a, const Math::TimelineItem<DS::Stage*> & b) {
    return a.GetLayer() < b.GetLayer();
}

dsOutputStage::dsOutputStage(DS::Data * data) : DS::Stage(data) {
    fboManager = NULL;
    fboTexture = NULL;
    aaProgram = NULL;
}

dsOutputStage::~dsOutputStage() {
    SafeDelete(fboManager);
    SafeDelete(fboTexture);
    SafeDelete(aaProgram);
}

void dsOutputStage::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    fboManager = dev->CreateRTManager(m_Data->GetWidth(), m_Data->GetHeight());
    fboTexture = dynamic_cast<Graph::Texture2D*>(
        dev->CreateTexture(Graph::TEXTURE_2D, m_Data->GetWidth(), m_Data->GetHeight(), 0, Graph::FORMAT_RGBA_8B, true)
        );
    fboManager->Attach(0, fboTexture);

    aaProgram = m_Data->LoadProgram("shader://output.cpp");
}


void dsOutputStage::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    std::list<Math::TimelineItem<DS::Stage*>> items;
    m_Stages.Get(time, &items);
    items.sort(compareTLItem);

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


void dsOutputStage::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;

    dev->Viewport(0,0, width, height);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_CULL_FACE);

    fboTexture->Enable();

    aaProgram->Enable();
    aaProgram->SetVariable4f("texResolution", width, height);
    aaProgram->SetVariable1f("time", t);

    float contrast = 0;
    float saturation = 0;
    float brightness = 0;

    aaProgram->SetVariable4f("contrast", contrast);
    aaProgram->SetVariable4f("saturation", saturation);
    aaProgram->SetVariable4f("brightness", brightness);
    
    dev->PushMatrix();
    dev->Scale(1, -1, 0);
    dev->Translate(0, -height, 0);
    shp->Square(0, 0, width, height, Math::Color4ub());
    dev->PopMatrix();

    aaProgram->Disable();

    fboTexture->Disable();
}
