#include "dsDisconnectScreen.h"

dsDisconnectScreen::dsDisconnectScreen(DS::Data * data) : DS::Stage(data) {

}

dsDisconnectScreen::~dsDisconnectScreen() {

}

void dsDisconnectScreen::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    sysLogo = m_Data->LoadCompound("model://logo.bxon");
    fog = m_Data->LoadProgram("shader://disfog.cpp");   
    
    sysLogo->Get()->GetObjectsWithLayer(&changeObj, Scene::LAYER_2);
    sysLogo->Get()->GetObjectsWithLayer(&refObj, Scene::LAYER_3);
    Mutate();
}

void dsDisconnectScreen::Update(int64_t start, int64_t end, int64_t time) {
    Mutate();
}

void dsDisconnectScreen::RenderFBO(int64_t start, int64_t end, int64_t time) {

}

void dsDisconnectScreen::Mutate() {
 
    for (int i = 0; i < changeObj.size(); i++) {
        float m = Math::RandomValue(0, 1);
        if (m > 0.99) {
            float r = Math::RandomValue(0, 1);
            Scene::Datablock * m = refObj[0]->GetData();
            if(r > 0.7)
                m = refObj[1]->GetData();
            else if (r > 0.5)
                m = refObj[2]->GetData();
            changeObj[i]->SetData(m);
        }
    }
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

    Scene::Object * objLogo = sysLogo->Get()->GetObject("logo");
    objLogo->Play(t * bFrameRate);

    Scene::Camera * cam = sysLogo->Get()->GetCamera("Camera");
    cam->SetAspect(aspect);
    cam->SetEnd(200);

    Math::Mat44 modelView = cam->GetMatrix();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);


    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    dev->LoadMatrix((float*)&modelView);


    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->PushMatrix();
    dev->Translate(0, t*0.5, 0);
    dev->FillMode(Graph::FILL_WIREFRAME);
    fog->Enable();
    sysLogo->Get()->Render(NULL, NULL, Scene::LAYER_2);
    fog->Disable();
    dev->PopMatrix();

    if (Math::RandomValue(0, 1) > 0.2)
        dev->FillMode(Graph::FILL_SOLID);
    else
        dev->FillMode(Graph::FILL_WIREFRAME);

    dev->Color(255, 0, 0);
    sysLogo->Get()->Render(NULL, NULL,Scene::LAYER_1);
     

    dev->FillMode(Graph::FILL_SOLID);
    dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    m_Data->GetShapeRenderer()->Square(0, 0, width, height, Math::Color4ub(0,0,0,Math::RandomValue(0,10)));

    dev->Enable(Graph::STATE_ZBUFFER_WRITE);
    dev->Enable(Graph::STATE_DEPTH_TEST);
}
