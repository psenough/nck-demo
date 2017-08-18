
#include "dsDisconnectTunnel.h"
#include "MaterialToProgram.h"
#include "nckMathUtils.h"

dsDisconnectTunnel::CorridorPart::CorridorPart(Graph::Device * dev, DS::Compound * c, Scene::LayerMask layer) {
    c->Get()->GetObjectsWithLayer(&lampsObjs,Scene::DATABLOCK_LAMP, layer);
    c->Get()->GetObjectsWithLayer(&modelsObjs, Scene::DATABLOCK_MODEL, layer);
    m_Device = dev;

    MaterialToProgram mtp(dev);
    mtp.SetEnableTransform(true);

    for (int i = 0; i < modelsObjs.size(); i++) {
        Scene::Object * o = modelsObjs[i];
        Scene::Model * m = dynamic_cast<Scene::Model*>(o->GetData());

        if (o->GetName().find("Porta.") != std::string::npos) {
            door = o;
        }

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

   if (door) {
       door->SetRotation(Math::EulerToQuat(Math::LinearInterpolation(Math::Vec3(0, 0, -115), Math::Vec3(0, 0, 0), anim/4.0)*M_PI / 180));
       door->Update();
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

dsDisconnectTunnel::dsDisconnectTunnel(DS::Data * data) : DS::Stage(data) {

}

dsDisconnectTunnel::~dsDisconnectTunnel() {
    
}

void dsDisconnectTunnel::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    corredor = m_Data->LoadCompound("model://corredores.bxon");
    controller = m_Data->LoadCompound("model://corredorControl.bxon");

    blocks.reserve(4);
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_1));
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_2));
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_3));
    blocks.push_back(CorridorPart(dev, corredor, Scene::LAYER_4));

    blocks_ids.reserve(100);
    blocks_ids.push_back(-1);
    blocks_ids.push_back(-1);

    for (int i = 0; i < 198; i++) {
        int val = floor(Math::RandomValue(0, 4));
        if (val == 4)
            val = 3;
        blocks_ids.push_back(val);
    }
}

void dsDisconnectTunnel::Update(int64_t start, int64_t end, int64_t time) {

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

    const Math::Vec3 restPosition = Math::Vec3(0, -42.0842, 11.7015);

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

    float speed = 10;
    float rot_x = 0.0;
    float rot_y = 0.0;
    float rot_z = 0.0;

    if (controller->Get()->GetObject("speed") != NULL) {
        Scene::Object * obj = controller->Get()->GetObject("speed");
        obj->Play(t * bFrameRate);
        speed = obj->GetPosition().GetX() * 10.0;
    }

    if (controller->Get()->GetObject("rotx") != NULL) {
        Scene::Object * obj = controller->Get()->GetObject("rotx");
        obj->Play(t * bFrameRate);
        rot_x = obj->GetPosition().GetX();
    }

    if (controller->Get()->GetObject("roty") != NULL) {
        Scene::Object * obj = controller->Get()->GetObject("roty");
        obj->Play(t * bFrameRate);
        rot_y = obj->GetPosition().GetX();
    }

    if (controller->Get()->GetObject("rotz") != NULL) {
        Scene::Object * obj = controller->Get()->GetObject("rotz");
        obj->Play(t * bFrameRate);
        rot_z = obj->GetPosition().GetX();
    }

    float dim_y = 50;
    float sT = t * speed;

    float dt = sT - floor(sT / dim_y) * dim_y;
    int offset = (int)floor(sT / dim_y);

    Math::Vec4 md = Math::Vec4(rot_x, rot_y, rot_z, 0);
    
    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_CULL_FACE);
   
    for (int i = 0; i < 4 && (i + offset) < blocks_ids.size(); i++) {
        int modelIndex = blocks_ids[i + offset];
        if (modelIndex < 0)
            continue;

        float anim = Math::Clamp(0, 4.0, dt / 10.0 + dim_y * 2-i *dim_y);
        if (i + offset <= 2)
            anim = 4.0;

        blocks[0].Render(matView, anim, Math::Vec4(restPosition.GetX(), dim_y*2.0-dim_y * i, -dt, 0), md);
    } 
}
