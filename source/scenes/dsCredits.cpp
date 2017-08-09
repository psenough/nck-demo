
#include "dsCredits.h"
#include "MaterialToProgram.h"

dsCredits::dsCredits(DS::Data * data) : DS::Stage(data) {

}

dsCredits::~dsCredits() {
    
}

void dsCredits::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    model = m_Data->LoadCompound("model://credits.bxon");
}

void dsCredits::Update(int64_t start, int64_t end, int64_t time) {
   
}

void dsCredits::RenderFBO(int64_t start, int64_t end, int64_t time) {
   
}


void dsCredits::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;

    Scene::Camera * cam = model->Get()->GetCamera("camCredits");

    Math::Mat44 modelView = cam->GetMatrix();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);
    
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    dev->LoadMatrix((float*)&modelView);
    
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    model->Get()->Render();
}
