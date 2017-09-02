#include "dsSceneLoading.h"

dsSceneLoading::dsSceneLoading(DS::Data * data) : DS::Stage(data) {

}

dsSceneLoading::~dsSceneLoading() {

}

void dsSceneLoading::Load() {
    logo = m_Data->LoadTexture("texture://tex2d_logo.png");
}

void renderTips(Graph::Device * dev) {
    dev->PushMatrix();
    
    dev->Rotate(45, 0, 1, 0);
    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->Vertex(-0.05, 0.0, 0.2);
    dev->Vertex(-0.05, 0.0, 0.05);
    dev->Vertex(0.05, 0.0, 0.05);
    dev->Vertex(0.05, 0.0, 0.2);   
    dev->End();

    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->Vertex(-0.05, 0.0, -0.05);
    dev->Vertex(-0.05, 0.0, -0.2);
    dev->Vertex(0.05, 0.0, -0.2);
    dev->Vertex(0.05, 0.0, -0.05);
    dev->End();

    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->Vertex(-0.2, 0.0, 0.05);
    dev->Vertex(-0.2, 0.0, -0.05);
    dev->Vertex(-0.05, 0.0, -0.05);
    dev->Vertex(-0.05, 0.0, 0.05); 
    dev->End();

    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->Vertex(0.2, 0.0, 0.05);
    dev->Vertex(0.05, 0.0, 0.05);
    dev->Vertex(0.05, 0.0, -0.05);
    dev->Vertex(0.2, 0.0, -0.05);
    dev->End();
    
    dev->PopMatrix();
}

void dsSceneLoading::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();

    dev->ClearColor(0.0, 0.0, 0.0, 1.0);
    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    dev->Perspective(aspect);
    dev->Enable(Graph::STATE_CULL_FACE);
    dev->CullMode(Graph::CULL_BACK);
  
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    Scene::Object camObj(dev);
    camObj.SetRotation(Math::EulerToQuat(Math::Vec3(90,0,0)* M_PI / 180.0));
    camObj.SetPosition(Math::Vec3(0, -5, 0.0));
    Scene::Camera cam(dev);
    cam.SetObject(&camObj);
    cam.Enable();


    dev->Disable(Graph::STATE_CULL_FACE);
  
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    const float logoSize = height * 0.8;
    dev->Disable(Graph::STATE_DEPTH_TEST);
    
    dev->Translate(0, -logoSize*0.06, 0);

    logo->Enable();
    shp->SetRoundMode(Gui::ShapeRenderer::ROUND_NONE);
    shp->Square(width/2 - logoSize * 0.5, height / 2 - logoSize * 0.5, logoSize, logoSize, Math::Color4ub());
    logo->Disable();

    // placeholder loading percentage.
    float percent = (time % 10000000) / 10e6;

    const float round = logoSize*0.01;
    shp->SetRoundMode(Gui::ShapeRenderer::ROUND_ALL);
    shp->SetRoundRadius(round);
    shp->Square(width / 2 - logoSize * 0.5, height / 2 + logoSize * 0.1, logoSize, logoSize * 0.05, Math::Color4ub());
    shp->Square(width / 2 - logoSize * 0.5+ round*0.5, height / 2 + logoSize * 0.1+ round*0.5, (logoSize- round)*percent, logoSize * 0.05- round, Math::Color4ub(0,0,0,255));

    shp->SetRoundMode(Gui::ShapeRenderer::ROUND_NONE);
}