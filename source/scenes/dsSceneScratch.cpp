
#include "dsSceneScratch.h"
#include "nckCamera.h"

dsSceneScratch::dsSceneScratch(DS::Data * data) : DS::Stage(data) {
    
}

dsSceneScratch::~dsSceneScratch() {

}

void dsSceneScratch::Load() {
    s1 = m_Data->LoadCompound("model://piramide.bxon");
    p = m_Data->LoadProgram("shader://simple.cpp");
}

void dsSceneScratch::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    Scene::Camera * cam = dynamic_cast<Scene::Camera*>(s1->Get()->GetDatablock(Scene::DATABLOCK_CAMERA, "Camera"));


    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    //cam->Enable(Graph::MATRIX_PROJECTION);
    dev->Perspective(m_Data->GetAspect());

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
    //cam->Enable(Graph::MATRIX_VIEW);

    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    Scene::Camera camera(dev);
    Scene::Object obj(dev);
    obj.SetPosition(Math::Vec3(0, -8, 0));
    obj.SetRotation(Math::EulerToQuat(Math::Vec3(M_PI / 2, 0, 0)));
    camera.SetObject(&obj);

    camera.Enable(Graph::MATRIX_VIEW);

    

    p->Enable();
    p->SetVariable1f("time", time / 1000.0f);
    s1->Get()->Render();
    p->Disable();

    /*int res = 128;


    float off = - 2* M_PI;

    for (int i = 0; i < res; i++) {
        float x0 = 4 * M_PI * (i-1) / res + off;
        float x = 4 * M_PI * i / res+ off;
        float x2 = 4 * M_PI * (i+1) / res+ off;

        float y0 = cos(x0);
        float y = cos(x);
        float y2 = cos(x2);

        dev->Color(255, 255, 255);
        dev->Begin(Graph::PRIMITIVE_LINES);
        dev->Vertex(x, 0, y);
        dev->Vertex(x2, 0, y2);
        dev->End();

        Math::Vec2 o1 = Math::Vec2(x, y) - Math::Vec2(x0, y0);
        Math::Vec2 o2 = Math::Vec2(x, y) - Math::Vec2(x2, y2);

        Math::Vec3 n =  Math::Cross(Math::Vec3(o1, 0), Math::Vec3(0, 0, -1));

     
        Math::Vec2 nn = Math::Normalize(n);

        dev->Color(0, 200, 255);
        dev->Begin(Graph::PRIMITIVE_LINES);
        dev->Vertex(x, 0, y);
        dev->Vertex(x+nn.GetX()*0.2, 0, y+nn.GetY()*0.2);
        dev->End();

    }*/
}