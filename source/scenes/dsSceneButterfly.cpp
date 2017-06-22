#include "dsSceneButterfly.h"

dsSceneButterfly::dsSceneButterfly(DS::Data * data) : DS::Stage(data) {
    butterfly = NULL;
    butterflyProgram = NULL;
}

dsSceneButterfly::~dsSceneButterfly() {
    SafeDelete(butterfly);
    SafeDelete(butterflyProgram);
}

void dsSceneButterfly::Load() {
    butterfly = m_Data->LoadCompound("model://butterfly.bxon");
    butterflyProgram = m_Data->LoadProgram("shader://butterfly.cpp");
}


void dsSceneButterfly::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();
  
    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1e6;

    dev->Viewport(0, 0, width, height);

    Scene::Camera * cam = butterfly->Get()->GetCamera("Camera");
    cam->GetObject()->Play(t * 24.0);

    cam->SetAspect(width / height);

    dev->ClearColor(0.0, 0.0, 0.0, 1.0);
    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    cam->Enable(Graph::MATRIX_VIEW); 
  
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    Scene::Object * butterfly_ref = butterfly->Get()->GetObject("Empty");
    butterfly_ref->Play(t * 24);


    Scene::Object * wing = butterfly->Get()->GetObject("Wing.L");
    float speed = 0.5;
    float dt = M_PI * (t - floor(t / speed) * speed)/ speed;

    wing->SetRotation(Math::EulerToQuat(Math::Vec3(0, sin(dt) * 60,0)*M_PI/180.0));
    wing->Update();

    wing = butterfly->Get()->GetObject("Wing.R");
    wing->SetRotation(Math::EulerToQuat(Math::Vec3(0, -sin(dt) * 60, 0)*M_PI / 180.0));
    wing->Update();

    //dev->PushMatrix();
//dev->Rotate(t*90, 0, 0, 1);
    butterflyProgram->Enable();
    butterfly->Get()->Render();
    butterflyProgram->Disable();
    //dev->PopMatrix();
}