
#include "dsSceneOutputFilter.h"

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

    // Render other stages.
    manager->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, m_Data->GetWidth(), m_Data->GetHeight());

        std::list<Math::TimelineItem<DS::Stage*>> items;
        m_Stages.Get(time / 1e6, &items); // Nota: Ir buscar em segundos
        ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
            i->GetObject()->Render(i->GetStart(), i->GetEnd(), time); // Nota: Aqui já é microsegundos.
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

    program->Enable();
    program->SetVariable4f("texResolution", m_Data->GetWidth(), m_Data->GetHeight());
    program->SetVariable4f("contrast", contrast);
    program->SetVariable4f("saturation", saturation);
    program->SetVariable4f("brightness", brightness);
    
    renderTexture->Enable();
    shp->Square(0, 0, m_Data->GetWidth(), m_Data->GetHeight(),Math::Color4ub());
    renderTexture->Disable();
    
    program->Disable();
}