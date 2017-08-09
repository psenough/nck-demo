
#include "dsDisconnectTunnel.h"
#include "MaterialToProgram.h"

dsDisconnectTunnel::dsDisconnectTunnel(DS::Data * data, dsInnerHouse * innerHouse) : DS::Stage(data) {
    this->innerHouse = innerHouse;
}

dsDisconnectTunnel::~dsDisconnectTunnel() {
    
}

void dsDisconnectTunnel::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
}

void dsDisconnectTunnel::Update(int64_t start, int64_t end, int64_t time) {
    //innerHouse->Update(start, end, time);
}

void dsDisconnectTunnel::RenderFBO(int64_t start, int64_t end, int64_t time) {
   
}


void dsDisconnectTunnel::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;


    const Math::Vec3 restPosition = Math::Vec3(0, -50.3652, 13.10984);

    Scene::Object objCam(dev);
    objCam.SetRotation(Math::EulerToQuat(Math::Vec3(90,0,0)*M_PI/180.0));
    objCam.SetPosition(restPosition);

    Scene::Camera cam(dev);
    cam.SetObject(&objCam);
    cam.SetFov(67.6927872);
    cam.SetAspect(aspect);
    cam.SetEnd(200);

    const Math::Mat44 matView = cam.GetMatrix();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam.Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->LoadMatrix((float*)&matView);

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    float dim_y = 50;
    float sT = t*10;

    float dt = sT - floor(sT / dim_y) * dim_y;
    int offset = (int)floor(sT / dim_y);

    Math::Vec4 md = Math::Vec4(sin(t*0.1)*0.5, cos(t*0.1), 0, 0) * Math::Clamp(0.0, 1.0, t / 10.0);

    innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), dim_y*2.0, -dt*2.0, 0), md);
    innerHouse->RenderCorridor(matView, Math::Clamp(0,4.33, dt / 10.0 + 100));

    innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), dim_y, -dt, 0), md);
    innerHouse->RenderCorridor(matView, Math::Clamp(0, 4.33, dt / 10.0 + 50));

    innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), 0, -dt, 0), md);
    innerHouse->RenderCorridor(matView, Math::Clamp(0, 4.33, dt / 10.0));

    innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), -dim_y, -dt, 0), md);
    innerHouse->RenderCorridor(matView, 0);
}
