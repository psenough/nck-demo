
#include "dsScene80Grid.h"
#include "nckCamera.h"

dsScene80Grid::dsScene80Grid(DS::Data * data) : DS::Stage(data) {
    
}

dsScene80Grid::~dsScene80Grid() {

}

void dsScene80Grid::Load() {
    grid = m_Data->LoadProgram("shader://SB_TILE_0002/grid.cpp");
    //circuit = dynamic_cast<Graph::Texture2D*>(m_Data->LoadTexture("texture://tex2d_circuits.jpg"));
    //square = dynamic_cast<Graph::Texture2D*>(m_Data->LoadTexture("texture://tex2d_squares.png"));
    //square->SetFilter(Graph::FilterType::FILTER_MIPMAPPING, Graph::FilterMode::FILTER_NONE);

}

static void RenderPlane(Graph::Device * dev, Math::Vec3 c, Math::Vec2 size) {
    dev->PushMatrix();
    dev->Translate(c.GetX(), c.GetY(), c.GetZ());
    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->TexCoord(0, 0);
    dev->Vertex(-size.GetX() / 2, size.GetY() / 2, 0);
    dev->TexCoord(0, 1);
    dev->Vertex(-size.GetX() / 2, -size.GetY() / 2, 0);
    dev->TexCoord(1, 1);
    dev->Vertex(size.GetX() / 2, -size.GetY() / 2, 0);
    dev->TexCoord(1, 0);
    dev->Vertex(size.GetX() / 2, size.GetY() / 2, 0);
    dev->End();
    dev->PopMatrix();
}

void dsScene80Grid::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    dev->ClearColor(1.0*1.2, 0.2*1.2, 0.5*1.2, 1.0);
    //dev->ClearColor(1.0*1.2, 0.5*1.2, 0.2*1.2, 1.0);
    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
 
    dev->Perspective(m_Data->GetAspect());

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    float angle = (time - floor(time / 40e6)*40e6) / 40e6;

    Scene::Camera camera(dev);
    Scene::Object obj(dev);
    obj.SetPosition(Math::Vec3(0, -8, 0));
    obj.SetRotation(Math::EulerToQuat(Math::Vec3(M_PI / 2, 2*angle * M_PI, 0)));
    camera.SetObject(&obj);

    camera.Enable(Graph::MATRIX_VIEW);

    dev->Disable(Graph::STATE_CULL_FACE);

  
    grid->Enable();
    
    grid->SetVariable1f("time", time / 1e6);

    RenderPlane(dev, Math::Vec3(0, 0, 10), Math::Vec2(1000, 1000));
    RenderPlane(dev, Math::Vec3(0, 0, -10), Math::Vec2(1000, 1000));

    grid->Disable();

}