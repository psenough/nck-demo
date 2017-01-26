
#include "dsSceneIcosphere.h"
#include "nckCamera.h"



dsSceneIcosphere::dsSceneIcosphere(DS::Data * data) : DS::Stage(data) {
    
}

dsSceneIcosphere::~dsSceneIcosphere() {

}

void dsSceneIcosphere::Load() {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    icosphere = m_Data->LoadCompound("model://icosphere.bxon", dynamic_cast<Scene::Processor*>(this));
    normal = m_Data->LoadProgram("shader://normal.cpp");
}


void dsSceneIcosphere::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

  
    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Perspective(m_Data->GetAspect());

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    Scene::Camera camera(dev);
    Scene::Object obj(dev);
    obj.SetPosition(Math::Vec3(0, -8, 0));
    obj.SetRotation(Math::EulerToQuat(Math::Vec3(M_PI / 2, 0, 0)));
    camera.SetObject(&obj);

    camera.Enable(Graph::MATRIX_VIEW);

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
  
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

  

    normal->Enable();
    normal->SetVariable1f("time", time / 1e6);
    icosphere->Get()->Render();
    normal->Disable();
}


Geometry::Mesh * dsSceneIcosphere::HandleGeometry(Geometry::Mesh * mesh) {
    return mesh;
}
