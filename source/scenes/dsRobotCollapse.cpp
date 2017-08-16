
#include "dsRobotCollapse.h"
#include "MaterialToProgram.h"
#include "dsLamps.h"

dsRobotCollapse::dsRobotCollapse(DS::Data * data) : DS::Stage(data) {

}   

dsRobotCollapse::~dsRobotCollapse() {
    
}

void dsRobotCollapse::HandleFinish(BXON::Map * map, Scene::Compound * compound) {
    camMarkers = Scene::Compound_Base::fetchCamerasWithKeyframes(map, dynamic_cast<Scene::Compound_Base*>(compound));
}


void dsRobotCollapse::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    MaterialToProgram * mtp = new MaterialToProgram(m_Data->GetGraphicsDevice());

    gynoid = m_Data->LoadCompound("model://gynoid2.bxon");
    gynoid->Get()->GetAllMaterials(&gynoidMats);
    for (int i = 0; i < gynoidMats.size(); i++) {
        Graph::Program * p = mtp->generate(gynoidMats[i]);
        gynoidMats[i]->SetProgram(p);
    }

    house = m_Data->LoadCompound("model://house_2.bxon",this);
    house->Get()->GetAllMaterials(&houseMats);
    for (int i = 0; i < houseMats.size(); i++) {
        Graph::Program * p = mtp->generate(houseMats[i]);
        houseMats[i]->SetProgram(p);
    }
}

Scene::Object * dsRobotCollapse::findNearestCamera(int keyframe) {
    std::vector< std::pair<float, Scene::Object*> > vec = camMarkers;

    if (camMarkers.size() == 0)
        return NULL;

    if (keyframe <= camMarkers[0].first)
        return camMarkers[0].second;
    else if (keyframe >= camMarkers[camMarkers.size() - 1].first)
        return camMarkers[camMarkers.size() - 1].second;

    for (int i = 0; i < camMarkers.size() - 1; i++) {
        if (keyframe >= camMarkers[i].first && keyframe <  camMarkers[i + 1].first) {
            return camMarkers[i].second;
        }
    }

    return NULL; // whut
}


void dsRobotCollapse::Update(int64_t start, int64_t end, int64_t time) {
    const float aspect = m_Data->GetAspect();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;

    Scene::Object * camObj = findNearestCamera(t * bFrameRate);

    if (camObj == NULL)
        return;

    Scene::Camera * cam = dynamic_cast<Scene::Camera*>(camObj->GetData());
    cam->SetAspect(aspect);

    camObj->Play(t * bFrameRate);
    camObj->Update();

    mainModelView = cam->GetMatrix();
    mainCamPos = camObj->GetPosition();
}

void dsRobotCollapse::RenderFBO(int64_t start, int64_t end, int64_t time) {
   
}


void dsRobotCollapse::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;

    Scene::Object * camObj = findNearestCamera(t * bFrameRate);

    if (camObj == NULL)
        return;

    Scene::Camera * cam = dynamic_cast<Scene::Camera*>(camObj->GetData());
    cam->SetAspect(aspect);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);


    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    dev->LoadMatrix((float*)&mainModelView);


    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
    
    LampConfig conf = LampConfig::generate(house, mainModelView);

    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_CULL_FACE);
    dev->CullMode(Graph::CULL_BACK);

    for (int i = 0; i < houseMats.size(); i++) {
        Scene::Material * mat = houseMats[i];
        Graph::Program * prog = mat->GetProgram();
        LampConfig::bind(conf, prog);
    } 

    for (int i = 0; i < gynoidMats.size(); i++) {
        Scene::Material * mat = gynoidMats[i];
        Graph::Program * prog = mat->GetProgram();
        LampConfig::bind(conf, prog);
    }

    house->Get()->Render();
    gynoid->Get()->Render();
}
