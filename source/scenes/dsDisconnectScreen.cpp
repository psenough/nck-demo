#include "dsDisconnectScreen.h"

dsDisconnectScreen::dsDisconnectScreen(DS::Data * data) : DS::Stage(data) {

}

dsDisconnectScreen::~dsDisconnectScreen() {

}

void dsDisconnectScreen::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    sysLogo = m_Data->LoadCompound("model://logo.bxon");

   
}

void dsDisconnectScreen::Update(int64_t start, int64_t end, int64_t time) {

}

void dsDisconnectScreen::RenderFBO(int64_t start, int64_t end, int64_t time) {

}


void dsDisconnectScreen::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;

    dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height);
    
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    m_Data->GetShapeRenderer()->Square(0, 0, width, height, Math::Color4ub(255, 255, 255));

    dev->Enable(Graph::STATE_ZBUFFER_WRITE);
    dev->Enable(Graph::STATE_DEPTH_TEST);

    Scene::Camera * cam = sysLogo->Get()->GetCamera("Camera");
    cam->SetAspect(aspect);

    Math::Mat44 modelView = cam->GetMatrix();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);


    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    dev->LoadMatrix((float*)&modelView);


    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Color(255, 0, 0);
    sysLogo->Get()->Render();
}
