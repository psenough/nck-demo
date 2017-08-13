
#include "dsDisconnectTunnel.h"
#include "MaterialToProgram.h"


dsDisconnectTunnel::CorridorPart::CorridorPart(Graph::Device * dev, DS::Compound * c, Scene::LayerMask layer) {
    c->Get()->GetObjectsWithLayer(&lampsObjs,Scene::DATABLOCK_LAMP, layer);
    c->Get()->GetObjectsWithLayer(&modelsObjs, Scene::DATABLOCK_MODEL, layer);
    m_Device = dev;

    MaterialToProgram mtp(dev);
    mtp.SetEnableTransform(true);

    for (int i = 0; i < modelsObjs.size(); i++) {
        Scene::Object * o = modelsObjs[i];
        Scene::Model * m = dynamic_cast<Scene::Model*>(o->GetData());

        std::vector<Scene::Material*> mVec = m->GetMaterials();

        for (int j = 0; j < mVec.size(); j++) {
            Scene::Material * m = mVec[j];

            if (std::find(mats.begin(), mats.end(), m) == mats.end()) {
                if (m->GetProgram()== NULL) {
                   Graph::Program * p = mtp.generate(m);
                   m->SetProgram(p);
                }
                mats.push_back(m);
            }
        }
    }
}


void dsDisconnectTunnel::CorridorPart::Render(const Math::Mat44 & mv, float anim, const Math::Vec4 & dist, const Math::Vec4 & modif) {
    lighting = LampConfig::generate(mv, lampsObjs);

   for (int i = 0; i < mats.size(); i++) {
        Scene::Material * mat = mats[i];
        Graph::Program * prog = mat->GetProgram();  
        LampConfig::bind(lighting, prog);
        prog->SetVariable4f("tr_modifier", modif.GetX(), modif.GetY(), modif.GetZ(), modif.GetW());
        prog->SetVariable4f("tr_distance", dist.GetX(), dist.GetY(), dist.GetZ(), dist.GetW());
    }

    for (int i = 0; i < modelsObjs.size(); i++) {
        Scene::Object * o = modelsObjs[i];
        Scene::Model * m = dynamic_cast<Scene::Model*>(o->GetData());

        m_Device->PushMatrix();
        o->Bind();
        m->Render(NULL);
        m_Device->PopMatrix();
    }
}

dsDisconnectTunnel::dsDisconnectTunnel(DS::Data * data, dsInnerHouse * innerHouse) : DS::Stage(data) {
    this->innerHouse = innerHouse;
}

dsDisconnectTunnel::~dsDisconnectTunnel() {
    
}

void dsDisconnectTunnel::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    corredor = m_Data->LoadCompound("model://corredores.bxon");

    blocks.reserve(4);
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_1));
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_2));
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_3));
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_4));


    blocks_ids.reserve(100);
    for (int i = 0; i < 100; i++) {
        int val = floor(Math::RandomValue(0, 4));
        if (val == 4)
            val = 3;
        blocks_ids.push_back(val);
    }
}

void dsDisconnectTunnel::Update(int64_t start, int64_t end, int64_t time) {
    //innerHouse->Update(start, end, time);
}

void dsDisconnectTunnel::RenderFBO(int64_t start, int64_t end, int64_t time) {
   
}


void dsDisconnectTunnel::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;

    /*Scene::Camera * objCam = corredor->Get()->GetCamera("Camera.001");
    objCam->SetAspect(aspect);
    Math::Mat44 mv = objCam->GetMatrix();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    objCam->Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->LoadMatrix((float*)&mv);

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_CULL_FACE);
    */

    /*dev->Color(255, 255, 255, 255);
    corredor->Get()->Render();
    */

    const Math::Vec3 restPosition = Math::Vec3(0, -50.3652, 13.10984);

    Scene::Object objCam(dev);
    objCam.SetRotation(Math::EulerToQuat(Math::Vec3(90,0,0)*M_PI/180.0));
    objCam.SetPosition(restPosition);

    Scene::Camera cam(dev);
    cam.SetObject(&objCam);
    cam.SetFov(67.6927872);
    cam.SetAspect(aspect);
    cam.SetEnd(200);

    const Math::Mat44 matView = cam.GetMatrix();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam.Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->LoadMatrix((float*)&matView);

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    float dim_y = 50;
    float sT = t*10;

    float dt = sT - floor(sT / dim_y) * dim_y;
    int offset = (int)floor(sT / dim_y);

    Math::Vec4 md = Math::Vec4(sin(t*0.1)*0.5, cos(t*0.1), 0, 0) * Math::Clamp(0.0, 1.0, t / 10.0);
    
    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_CULL_FACE);
    
    //int offset = (int)floor(sT / dim_y);

    //blocks[0].Render(mv, 0.0,Math::Vec4(), Math::Vec4());

    for (int i = 0; i < 4; i++) {
        int modelIndex = blocks_ids[i + offset];
        blocks[modelIndex].Render(matView, 0.0, Math::Vec4(restPosition.GetX(), dim_y*2.0-dim_y * i, -dt, 0), md);
    } 

    /*innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), dim_y*2.0, -dt*2.0, 0), md);
    innerHouse->RenderCorridor(matView, Math::Clamp(0,4.33, dt / 10.0 + 100));

    innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), dim_y, -dt, 0), md);
    innerHouse->RenderCorridor(matView, Math::Clamp(0, 4.33, dt / 10.0 + 50));

    innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), 0, -dt, 0), md);
    innerHouse->RenderCorridor(matView, Math::Clamp(0, 4.33, dt / 10.0));

    innerHouse->UpdateCorridorTransform(Math::Vec4(restPosition.GetX(), -dim_y, -dt, 0), md);
    innerHouse->RenderCorridor(matView, 0);
    */

}
