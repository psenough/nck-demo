
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
  
    program = m_Data->LoadProgram("shader://output/fxaa_brightness_contrast.cpp");

    manager = m_Data->GetGraphicsDevice()->CreateRTManager(m_Data->GetWidth(), m_Data->GetHeight());
    renderTexture = dynamic_cast<Graph::Texture2D*>(
        m_Data->GetGraphicsDevice()->CreateTexture(Graph::TEXTURE_2D, m_Data->GetWidth(), m_Data->GetHeight(), 0, Graph::FORMAT_RGBA_8B, true)
        );

    manager->Attach(0, renderTexture);
}


void dsSceneOutputFilter::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();


    // Render other stages FBO
    std::list<Math::TimelineItem<DS::Stage*>> items;
    m_Stages.Get(time, &items);
    ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
        i->GetObject()->RenderFBO(i->GetStart(), i->GetEnd(), time);
    }

    // Render other stages
    manager->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, m_Data->GetWidth(), m_Data->GetHeight());
        ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
            i->GetObject()->Render(i->GetStart(), i->GetEnd(), time); 
        }
    }
    manager->Disable();

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
    program->SetVariable4f("texResolution", m_Data->GetWidth(), m_Data->GetHeight());
    program->SetVariable4f("contrast", contrast);
    program->SetVariable4f("saturation", saturation);
    program->SetVariable4f("brightness", brightness);
    
    renderTexture->Enable();
    dev->Color(255, 255, 255);
    Square(m_Data->GetWidth(), m_Data->GetHeight(), manager->InvertedY());
    renderTexture->Disable();
    
    program->Disable();

    float alpha = Math::RandomValue(0, 255);

    dev->Color(0, 0, 0, alpha*0.05);
    Square(m_Data->GetWidth(), m_Data->GetHeight(), false);

    dev->Color(255, 255, 255);

    int div = 8;
    int dim = m_Data->GetWidth() / div;
    int squares = div * m_Data->GetWidth() / 1920;


    for (int x = (m_Data->GetWidth() - (dim-1) * div); x < m_Data->GetWidth(); x += dim)
    {
        for (int y = (m_Data->GetHeight() - (dim-1) * div); y < m_Data->GetHeight(); y += dim)
        {
            m_Data->GetShapeRenderer()->Square(x, y, squares, squares, Math::Color4ub(30, 30, 30, alpha));
        }
    }
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