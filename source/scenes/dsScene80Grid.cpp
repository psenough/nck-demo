
#include "dsScene80Grid.h"
#include "nckCamera.h"
#include "../dsUtils.h"


dsScene80Grid::dsScene80Grid(DS::Data * data) : dsSceneFloatPopup(data){
    rtTexture = NULL;
    rtManager = NULL;
}

dsScene80Grid::~dsScene80Grid() {
    SafeDelete(rtTexture);
    SafeDelete(rtManager);
}

void dsScene80Grid::Load() {
    grid = m_Data->LoadProgram("shader://SB_TILE_0002/grid.cpp");
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    rtTexture = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, 256, 256, 0, Graph::FORMAT_RGBA_8B, true));
    rtTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    rtTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    rtTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NONE);
    rtTexture->SetAdressMode(Graph::ADRESS_CLAMP);
    rtManager = dev->CreateRTManager(256, 256);
    rtManager->Attach(0, rtTexture);

    menger = m_Data->LoadCompound("model://menger.bxon");

    basicProg = m_Data->LoadProgram("shader://map/basic.cpp");
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

void dsScene80Grid::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    rtManager->Enable();
    {
        
        dev->Clear();
        dev->Viewport(0, 0, rtTexture->GetWidth(), rtTexture->GetHeight());

        dev->Disable(Graph::STATE_DEPTH_TEST);
        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Ortho2D(rtTexture->GetWidth(), rtTexture->GetHeight());
        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();     
        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        dev->Color(255, 250, 240);
        DS::RenderSquare(dev, rtTexture->GetWidth(), rtTexture->GetHeight(), false);

        dev->Enable(Graph::STATE_DEPTH_TEST);

        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Perspective(512.0/512.0);

        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();
       
        float angle = (time - floor(time / 40e6)*40e6) / 40e6;
        Scene::Camera camera(dev);
        Scene::Object obj(dev);
        obj.SetPosition(Math::Vec3(0, -8, 0));

        obj.SetRotation(Math::EulerToQuat(Math::Vec3(M_PI / 2, 2 * angle * M_PI, 0)));
        camera.SetObject(&obj);
        camera.Enable();

        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        dev->Enable(Graph::STATE_BLEND);
        dev->Enable(Graph::STATE_DEPTH_TEST);

        dev->PushMatrix();
        float f_time = (time - start) / 1e6;
        basicProg->Enable();
        dev->Rotate(f_time*20, 1, 1, 1);
        menger->Get()->Render();
        basicProg->Disable();
        dev->PopMatrix();

        dev->Disable(Graph::STATE_CULL_FACE);
        grid->Enable();
        grid->SetVariable1f("time", time / 1e6);
        RenderPlane(dev, Math::Vec3(0, 0, 10), Math::Vec2(1000, 1000));
        RenderPlane(dev, Math::Vec3(0, 0, -10), Math::Vec2(1000, 1000));
        grid->Disable();
       
    }
    rtManager->Disable();
}

void dsScene80Grid::Render(int64_t start, int64_t end, int64_t time) {

    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    prepare(start, end, time);

    Math::Vec2 pos = Math::LinearInterpolation(showPosA, showPosB, alpha);

    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->PushMatrix();
    dev->Translate(pos.GetX(), pos.GetY(), 0);

    dev->Color(255, 255, 255, 240);
    rtTexture->Enable();
    DS::RenderSquare(dev, width, alpha*height, false);
    rtTexture->Disable();

    drawBorders();



}