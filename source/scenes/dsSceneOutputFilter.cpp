
#include "dsSceneOutputFilter.h"
#include "dsSceneLoading.h"

dsSceneOutputFilter::dsSceneOutputFilter(DS::Data * data) : DS::Stage(data) {
    contrast = 0;
    saturation = 0;
    brightness = 0;
    manager = NULL;
    renderTexture = NULL;
}

dsSceneOutputFilter::~dsSceneOutputFilter() {
    SafeDelete(manager);
}

void dsSceneOutputFilter::Load() {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    manager = dev->CreateRTManager(m_Data->GetWidth(), m_Data->GetHeight());
    renderTexture = dynamic_cast<Graph::Texture2D*>(
        dev->CreateTexture(Graph::TEXTURE_2D, m_Data->GetWidth(), m_Data->GetHeight(), 0, Graph::FORMAT_RGBA_8B, true)
        );

    manager->Attach(0, renderTexture);

    accumManager = dev->CreateRTManager(m_Data->GetWidth(), m_Data->GetHeight());
    accumTexture = dynamic_cast<Graph::Texture2D*>(
        dev->CreateTexture(Graph::TEXTURE_2D, m_Data->GetWidth(), m_Data->GetHeight(), 0, Graph::FORMAT_RGBA_8B, true)
        );
    accumManager->Attach(0, accumTexture);

    program = m_Data->LoadProgram("shader://output.cpp");
    bugfix = m_Data->LoadProgram("shader://bugfix.cpp");
}


void dsSceneOutputFilter::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    // Render other stages FBO
    std::list<Math::TimelineItem<DS::Stage*>> items;
    m_Stages.Get(time, &items);
      
    accumManager->Enable();
    {
        // Where we are going, we don't need clear :D
        //dev->Clear();
        dev->Viewport(0, 0, renderTexture->GetWidth(), renderTexture->GetHeight());

        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Ortho2D(renderTexture->GetWidth(), renderTexture->GetHeight());
        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();
        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        dev->Enable(Graph::STATE_BLEND);
        dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);
        dev->Disable(Graph::STATE_DEPTH_TEST);
        dev->Disable(Graph::STATE_CULL_FACE);
        dev->Disable(Graph::STATE_ZBUFFER_WRITE);

        bugfix->Enable();
        bugfix->SetVariable1f("alpha",255);
        renderTexture->Enable();
        //dev->Color(255, 255, 255, 220);
        Square(renderTexture->GetWidth(), renderTexture->GetHeight(), accumManager->InvertedY());
        renderTexture->Disable();
        bugfix->Disable();
    }
    accumManager->Disable();

    // Render other stages
    manager->Enable();
    {
        // Where we are going, we don't need clear :D
        if(Math::RandomValue(0,1)>0.2)
            dev->Clear();
        dev->Viewport(0, 0, accumTexture->GetWidth(), accumTexture->GetHeight());

        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Ortho2D(accumTexture->GetWidth(), accumTexture->GetHeight());
        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();
        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        dev->Enable(Graph::STATE_BLEND);
        dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);
        dev->Disable(Graph::STATE_DEPTH_TEST);
        dev->Disable(Graph::STATE_CULL_FACE);
        dev->Disable(Graph::STATE_ZBUFFER_WRITE);

        bugfix->Enable();
        bugfix->SetVariable1f("alpha", 220);
        accumTexture->Enable();
        //dev->Color(255, 255, 255, 240);
        Square(accumTexture->GetWidth(), accumTexture->GetHeight(), manager->InvertedY());
        accumTexture->Disable();
        bugfix->Disable();

        ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
            i->GetObject()->RenderFBO(i->GetStart(), i->GetEnd(), time);
        }

    }
    manager->Disable();
      


    dev->Enable(Graph::STATE_BLEND);

    // Apply final filters and present
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->Disable(Graph::STATE_CULL_FACE);

    program->Enable();
    //dev->Color(255, 255, 255);
    renderTexture->Enable();
    Square(m_Data->GetWidth(), m_Data->GetHeight(), manager->InvertedY());
    renderTexture->Disable();
    program->Disable();

}

void dsSceneOutputFilter::Square(float w, float h, bool invert) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->TexCoord(0, invert?1:0);
    dev->Vertex(0, 0);
    dev->TexCoord(0, invert?0:1);
    dev->Vertex(0, h);
    dev->TexCoord(1, invert?0:1);
    dev->Vertex(w, h);
    dev->TexCoord(1, invert?1:0);
    dev->Vertex(w, 0);
    dev->End();
}