
#include "dsRobotCollapse.h"
#include "MaterialToProgram.h"
#include "dsLamps.h"

dsRobotCollapse::dsRobotCollapse(DS::Data * data) : DS::Stage(data) {

}   

dsRobotCollapse::~dsRobotCollapse() {
    
}

void dsRobotCollapse::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    model = m_Data->LoadCompound("model://robotCollapse.bxon");

    MaterialToProgram * mtp = new MaterialToProgram(m_Data->GetGraphicsDevice());

    model->Get()->GetAllMaterials(&mats);
    for (int i = 0; i < mats.size(); i++) {
        Graph::Program * p = mtp->generate(mats[i]);
        mats[i]->SetProgram(p);
    }
}

void dsRobotCollapse::Update(int64_t start, int64_t end, int64_t time) {

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


    Scene::Camera * cam = model->Get()->GetCamera("robotCam");
 
    Math::Mat44 modelView = cam->GetMatrix();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);


    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    dev->LoadMatrix((float*)&modelView);


    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();


    LampConfig conf = LampConfig::generate(model, modelView);

    for (int i = 0; i < mats.size(); i++) {
        Scene::Material * mat = mats[i];
        Graph::Program * prog = mat->GetProgram();
        LampConfig::bind(conf, prog);
    }

    model->Get()->Render();
}
