
#include "dsSceneMetaballs.h"
#include "nckCamera.h"
#include "../dsUtils.h"


dsSceneMetaballs::dsSceneMetaballs(DS::Data * data) : dsSceneFloatPopup(data){
    rtTexture = NULL;
    rtManager = NULL;
    spheres.push_back(Scene::MCSphereShape());
    spheres.push_back(Scene::MCSphereShape());
    spheres.push_back(Scene::MCSphereShape());
    spheres.push_back(Scene::MCSphereShape());
    processor = new Scene::MCRenderer(m_Data->GetGraphicsDevice(), 20, 20, 6);
}

dsSceneMetaballs::~dsSceneMetaballs() {
    SafeDelete(rtTexture);
    SafeDelete(rtManager);
}

void dsSceneMetaballs::Load() {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    rtTexture = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, 256, 256, 0, Graph::FORMAT_RGBA_8B, true));
    rtTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    rtTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    rtTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NONE);
    rtTexture->SetAdressMode(Graph::ADRESS_CLAMP);
    rtManager = dev->CreateRTManager(rtTexture->GetWidth(), rtTexture->GetHeight());
    rtManager->Attach(0, rtTexture);

    basicProg = m_Data->LoadProgram("shader://basic.cpp");
    //gynoid = m_Data->LoadCompound("model://gynoid.bxon");
    //materialProg = m_Data->LoadProgram("shader://material.cpp");
    //basicProg = m_Data->LoadProgram("shader://map/basic.cpp");

    //dynamic_cast<Scene::Material*>(gynoid->Get()->GetDatablock(Scene::DATABLOCK_MATERIAL, "Grey"))->SetProgram(materialProg);
    //dynamic_cast<Scene::Material*>(gynoid->Get()->GetDatablock(Scene::DATABLOCK_MATERIAL, "Black"))->SetProgram(materialProg);
    //dynamic_cast<Scene::Material*>(gynoid->Get()->GetDatablock(Scene::DATABLOCK_MATERIAL, "Eyes"))->SetProgram(materialProg);

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

void dsSceneMetaballs::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    rtManager->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, rtTexture->GetWidth(), rtTexture->GetHeight());

        dev->Disable(Graph::STATE_ZBUFFER_WRITE);
        dev->Disable(Graph::STATE_DEPTH_TEST);
        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Ortho2D(rtTexture->GetWidth(), rtTexture->GetHeight());
        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();     
        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        dev->Color(255, 255, 255);
        DS::RenderSquare(dev, rtTexture->GetWidth(), rtTexture->GetHeight(), false);





        dev->Enable(Graph::STATE_ZBUFFER_WRITE);
        dev->Enable(Graph::STATE_DEPTH_TEST);


        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Perspective(rtTexture->GetWidth() / rtTexture->GetHeight());

        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();

        float angle = (time - floor(time / 40e6)*40e6) / 40e6;
        Scene::Camera camera(dev);
        Scene::Object obj(dev);
        obj.SetPosition(Math::Vec3(0, -8, 0));

        obj.SetRotation(Math::EulerToQuat(Math::Vec3(M_PI / 2, 2 * angle * M_PI, 0)));
        camera.SetObject(&obj);
        camera.Enable();

        //dev->MatrixMode(Graph::MATRIX_PROJECTION);
        //dev->Identity();
        //Scene::Camera * cam = dynamic_cast<Scene::Camera*>(gynoid->Get()->GetDatablock(Scene::DATABLOCK_CAMERA, "Camera"));
        //cam->SetAspect(rtTexture->GetWidth() / rtTexture->GetHeight());
        //cam->Enable(Graph::MATRIX_PROJECTION);

        //dev->MatrixMode(Graph::MATRIX_VIEW);
        //dev->Identity();
        //cam->Enable();

        //float angle = (time - floor(time / 2e6)*2e6)/1e6-1;

     


        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();
        //dev->Rotate(angle, 0, 0, 1);

        dev->Enable(Graph::STATE_BLEND);
        dev->Enable(Graph::STATE_DEPTH_TEST);

        //basicProg->Enable();
        //materialProg->Enable();
        //materialProg->SetVariable4f("lamp_mv_pos", 0, 0, 0, 0);

        dev->FillMode(Graph::FILL_WIREFRAME);
        dev->Color(0, 0, 0);
        //gynoid->Get()->Render();
        
        dev->FillMode(Graph::FILL_SOLID);

        //materialProg->Disable();
        //basicProg->Disable();

        dev->PushMatrix();
       
        processor->Clear();

        float zRotation = (time - start) / 1.0e6;

        spheres[0] = Scene::MCSphereShape(Math::Vec3(sin(zRotation + 3.14), cos(zRotation) * 2, -cos(zRotation)), 0.5);
        spheres[1] = Scene::MCSphereShape(Math::Vec3(cos(zRotation), sin(zRotation) * 2, cos(zRotation)), 0.5);
        spheres[2] = Scene::MCSphereShape(Math::Vec3(cos(zRotation)*2.1, sin(zRotation)*1.2, cos(zRotation + 2.24)*1.4), 0.5);
        spheres[3] = Scene::MCSphereShape(Math::Vec3(0, 0, 0), 1);

        processor->ApplySpheres(spheres);
        processor->Update();

  
        dev->Rotate(zRotation, 0, 1, 0);
        //dev->Scale(0.1, 0.1, 0.1);

        basicProg->Enable();
        processor->Render();
        basicProg->Disable();

        dev->PopMatrix();

        /*
        dev->Disable(Graph::STATE_CULL_FACE);
        grid->Enable();
        grid->SetVariable1f("time", time / 1e6);
        RenderPlane(dev, Math::Vec3(0, 0, 10), Math::Vec2(1000, 1000));
        RenderPlane(dev, Math::Vec3(0, 0, -10), Math::Vec2(1000, 1000));
        grid->Disable();
        */
       
    }
    rtManager->Disable();
}

void dsSceneMetaballs::Render(int64_t start, int64_t end, int64_t time) {

    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    prepare(start, end, time);

    Math::Vec2 pos = Math::LinearInterpolation(showPosA, showPosB, alpha);

    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->PushMatrix();
    dev->Translate(pos.GetX(), pos.GetY(), 0);

    dev->Color(255, 255, 255, 240);
    rtTexture->Enable();
    DS::RenderSquare(dev, width, alpha*height, true);
    rtTexture->Disable();

    drawBorders();
}