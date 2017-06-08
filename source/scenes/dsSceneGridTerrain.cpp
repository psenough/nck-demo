#include "dsSceneGridTerrain.h"

dsSceneGridTerrain::dsSceneGridTerrain(DS::Data * data) : DS::Stage(data) {

}

dsSceneGridTerrain::~dsSceneGridTerrain() {

}

void dsSceneGridTerrain::Load() {
    terrainProgram = m_Data->LoadProgram("shader://terreno.cpp");
    terrainModel = m_Data->LoadCompound("model://terreno.bxon");
    
}


void dsSceneGridTerrain::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

  
    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1e6;

    dev->Viewport(0, 0, width, height);

    Scene::Camera * cam = terrainModel->Get()->GetCamera("Camera");
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

    terrainProgram->Enable();
    terrainModel->Get()->Render();
    terrainProgram->Disable();
}