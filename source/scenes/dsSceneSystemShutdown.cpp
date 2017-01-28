
#include "dsSceneSystemShutdown.h"
#include "../dsUtils.h"
#include <algorithm>

dsSceneSysShutdown::dsSceneSysShutdown(DS::Data * data) : DS::Stage(data) {

}

dsSceneSysShutdown::~dsSceneSysShutdown() {

}

void dsSceneSysShutdown::Load() {
    sysLogo = m_Data->LoadCompound("model://logo_machine.bxon");
}

void dsSceneSysShutdown::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();

    dev->ClearColor(0.0, 0.0, 0.0, 1.0);
  

    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);
    dev->Disable(Graph::STATE_CULL_FACE);
    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height, 0, 1);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    shp->Square(0, 0, width, height, Math::Color4ub(255, 255, 255));


    /*
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Perspective(width / height);
  
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dynamic_cast<Scene::Camera*>(brain->Get()->GetDatablock(Scene::DATABLOCK_CAMERA, "Camera"))->Enable();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->FillMode(Graph::PolygonMode::FILL_WIREFRAME);
    dev->Color(0, 0, 0);
    dev->Rotate(time / 1e6, 0, 0, 1);
    
    builder->Enable();
    //builder->SetVariable1f("time", (time-start) / 1e6);
    //menger->Get()->Render();
    //brain->Get()->Render();

    float f_time = (time - start) / 1e6;

    if (f_time >= 0 && f_time < 13){
        builder->SetVariable1f("time", f_time);
        cube->Get()->Render();
    }
    else if (f_time >= 13 && f_time < 26){
        builder->SetVariable1f("time", f_time - 13);
        sphere->Get()->Render();
    }
    else {
        builder->SetVariable1f("time", f_time - 26);
        brain->Get()->Render();
    }
    builder->Disable();
    */
        
    dev->FillMode(Graph::PolygonMode::FILL_SOLID);
    

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height, 0, 1);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
      
    float scaleFactor = width / 1920;

    dev->PushMatrix();
    dev->Translate(width / 2, height / 2, 0);
    dev->Scale(300* scaleFactor,-300* scaleFactor,300* scaleFactor);
    dev->Color(200, 0, 0);
    sysLogo->Get()->Render();
    dev->PopMatrix();

    float alpha = Math::RandomValue(0, 20);
    dev->Color(30, 30, 30, alpha);
    DS::RenderCrossesMatrix(dev, m_Data->GetWidth(), m_Data->GetHeight());
}

