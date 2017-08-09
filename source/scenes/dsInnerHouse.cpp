
#include "dsInnerHouse.h"
#include "MaterialToProgram.h"

dsInnerHouse::dsInnerHouse(DS::Data * data) : DS::Stage(data) {     
    mc_renderer = NULL;
}

dsInnerHouse::~dsInnerHouse() {
    SafeDelete(mc_renderer);
}

void dsInnerHouse::HandleFinish(BXON::Map * map, Scene::Compound * compound) {
    camMarkers = Scene::Compound_Base::fetchCamerasWithKeyframes(map, dynamic_cast<Scene::Compound_Base*>(compound));
}

void dsInnerHouse::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    mc_renderer = new Scene::MCRenderer(dev, 40, 30, 6);
    mc_spheres.push_back(Scene::MCSphereShape());
    mc_spheres.push_back(Scene::MCSphereShape());
    //mc_spheres.push_back(Scene::MCSphereShape());

    mc_spheres[0] = Scene::MCSphereShape(Math::Vec3(0, 0, 0.0),1.0);
    mc_spheres[1] = Scene::MCSphereShape(Math::Vec3(-1.0, -1.0, -0.25), 0.25);
    //mc_spheres[2] = Scene::MCSphereShape(Math::Vec3(1.0, -1.0, -0.25), 0.25);

    mc_renderer->ApplySpheres(mc_spheres);
    mc_renderer->Update();

    cbMatProgram = m_Data->LoadProgram("shader://cubemap.cpp");

    mainHouse = m_Data->LoadCompound("model://house_inner_1.bxon");
    corridor = m_Data->LoadCompound("model://house_inner_2.bxon");
    cameras = m_Data->LoadCompound("model://house_inner_cam.bxon", this);
    butterfly = m_Data->LoadCompound("model://butterfly.bxon");

    mainHouse->Get()->GetAllMaterials(&mats_house);
    corridor->Get()->GetAllMaterials(&mats_corridor);
  
    MaterialToProgram * mtp = new MaterialToProgram(m_Data->GetGraphicsDevice());

    std::vector<Scene::Texture*> t1;
    if (mainHouse->Get()->GetAllTextures(&t1) > 0){
        for (int i = 0; i < t1.size(); i++) {
            t1[i]->GetTexture()->SetAnisotropyFilter(8);
        }
    }

    std::vector<Scene::Texture*> t2;
    if (corridor->Get()->GetAllTextures(&t2) > 0) {
        for (int i = 0; i < t2.size(); i++) {
            t2[i]->GetTexture()->SetAnisotropyFilter(8);
        }
    }  
    
    for (int i = 0; i < mats_house.size(); i++) {
        Graph::Program * p = mtp->generate(mats_house[i]);
        mats_house[i]->SetProgram(p);
    }

    mtp->SetEnableTransform(true);
    for (int i = 0; i < mats_corridor.size(); i++) {
        Graph::Program * p = mtp->generate(mats_corridor[i]);
        mats_corridor[i]->SetProgram(p);
    }


    int faceSize = 256;

    cbRT = dev->CreateRTManager(faceSize, faceSize);

    cbTex = dynamic_cast<Graph::TextureCubeMap*>(dev->CreateTexture(Graph::TEXTURE_CUBEMAP, faceSize, faceSize, 0, Graph::FORMAT_RGBA_8B, true));
    cbTex->SetAdressMode(Graph::ADRESS_WRAP);
    cbTex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_LINEAR);
    cbTex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_LINEAR);

    cbRT->Attach(0, cbTex);
}

void dsInnerHouse::Update(int64_t start, int64_t end, int64_t time) {
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

    butterfly->Get()->GetObject("Borboleta")->Play(t * bFrameRate);

    {
        Scene::Object * wing = butterfly->Get()->GetObject("B.Wing.L");
        float speed = 0.5;
        float dt = M_PI * (t - floor(t / speed) * speed) / speed;

        wing->SetRotation(Math::EulerToQuat(Math::Vec3(-90 + sin(dt) * 60, 0, 0)*M_PI / 180.0));
        wing->Update();

        wing = butterfly->Get()->GetObject("B.Wing.R");
        wing->SetRotation(Math::EulerToQuat(Math::Vec3(90 - sin(dt) * 60, 0, 0)*M_PI / 180.0));
        wing->Update();

        Scene::Object * objVent = mainHouse->Get()->GetObject("Ventoinha");
        objVent->SetRotation(Math::EulerToQuat(Math::Vec3(0, 0, t)));
        objVent->Update();
    }

    Scene::Object * sphere = mainHouse->Get()->GetObject("MetaPlace");
    metaPos = sphere->GetPosition();

    corridor->Get()->GetObject("Porta")->Play(t*bFrameRate);
}

Scene::Object * dsInnerHouse::findNearestCamera(int keyframe) {
    std::vector< std::pair<float, Scene::Object*> > vec = camMarkers;

    if (camMarkers.size() == 0)
        return NULL;

    if (keyframe <= camMarkers[0].first)
        return camMarkers[0].second;
    else if (keyframe >= camMarkers[camMarkers.size() - 1].first)
        return camMarkers[camMarkers.size() - 1].second;

    for (int i = 0; i < camMarkers.size()-1; i++) {
        if ( keyframe >= camMarkers[i].first && keyframe <  camMarkers[i + 1].first) {
            return camMarkers[i].second;
        }
    }

    return NULL; // whut
}


void dsInnerHouse::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    Scene::Object * sphere = mainHouse->Get()->GetObject("MetaPlace");
    for (int i = 0; i < 6; i++)
        RenderCubeMap(sphere->GetPosition(), i, this);

}

dsInnerHouse::LampConfig & dsInnerHouse::generateLampConfig(DS::Compound * c, Math::Mat44 viewMatrix) {
    std::vector<Scene::Object*> objects;
    c->Get()->GetObjectsWithLayer(&objects, Scene::DATABLOCK_LAMP);

    dsInnerHouse::LampConfig r;

    //Math::Vec4 lamp_pos[8]; // position, energy
    //Math::Vec4 lamp_dir[8]; // normal, type (0 - point, 1 - spot, 2 - sun, 3 -area) 
    //Math::Vec4 lamp_color[8]; // rgb, distance
    //Math::Vec4 lamp_params[8]; // ????

    for (size_t i = 0; i < objects.size() && i < 8; i++) {
        Scene::Object * obj = objects[i];
        Scene::Lamp * lamp = dynamic_cast<Scene::Lamp*>(obj->GetData());

        Math::Vec4 pMV = Math::Vec4(obj->GetPosition(), 1.0) * viewMatrix;
        Math::Vec4 dMV = Math::Vec4(0, 0, 0, 0);

        if (lamp->GetType() == Scene::LAMP_TYPE_SPOT || lamp->GetType() == Scene::LAMP_TYPE_SUN)
            dMV = (Math::Vec4(0, 0, 1, 0) * obj->GetMatrix()) * viewMatrix;

        r.lamp_pos[i] = Math::Vec4(Math::Vec3(pMV), lamp->GetEnergy());
        r.lamp_dir[i] = Math::Vec4(Math::Vec3(dMV), (float)lamp->GetLampType());
        r.lamp_color[i] = Math::Vec4(lamp->GetColor().GetR(), lamp->GetColor().GetG(), lamp->GetColor().GetB(), lamp->GetDistance());
        r.lamp_params[i] = Math::Vec4();
    }

    if (objects.size() < 8)
        r.lamp_pos[objects.size()] = Math::Vec4(0, 0, 0, -1); // early terminator.
    
    return r;
}

void dsInnerHouse::bindLampConfigToProg(LampConfig & config, Graph::Program * prog) {
    prog->SetArray4f("lamp_pos_mv", 8, (float*)config.lamp_pos);
    prog->SetArray4f("lamp_dir_mv", 8, (float*)config.lamp_dir);
    prog->SetArray4f("lamp_color", 8, (float*)config.lamp_color);
    prog->SetArray4f("lamp_params", 8, (float*)config.lamp_params);
}

void dsInnerHouse::Render(int64_t start, int64_t end, int64_t time) {
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

    RenderFromView(mainModelView);

    RenderMeta();
}

void dsInnerHouse::RenderFromView(const Math::Mat44 & viewMatrix) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    
    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_CULL_FACE);

    LampConfig houseLamps = generateLampConfig(mainHouse, viewMatrix);
    for (int i = 0; i < mats_house.size(); i++) {
        Scene::Material * mat = mats_house[i];
        Graph::Program * prog = mat->GetProgram();
        bindLampConfigToProg(houseLamps, prog);
    }

    LampConfig corridorLamps = generateLampConfig(corridor, viewMatrix);
    for (int i = 0; i < mats_corridor.size(); i++) {
        Scene::Material * mat = mats_corridor[i];
        Graph::Program * prog = mat->GetProgram();
        bindLampConfigToProg(corridorLamps, prog);
    }

    UpdateCorridorTransform(Math::Vec4(0, 0, 0, 0), Math::Vec4(0, 0, 0, 0));

    RenderButterfly();
    mainHouse->Get()->Render();
    corridor->Get()->Render();
}

void dsInnerHouse::RenderButterfly() {
    butterfly->Get()->Render();
}

void dsInnerHouse::RenderMeta() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    dev->PushMatrix();
    dev->Translate(metaPos.GetX(), metaPos.GetY(), metaPos.GetZ());
    cbTex->Enable();
    cbMatProgram->Enable();
    cbMatProgram->SetVariable4f("gphCameraPos", mainCamPos.GetX(), mainCamPos.GetY(), mainCamPos.GetZ());
    mc_renderer->Render();
    cbTex->Disable();
    cbMatProgram->Disable();
    dev->PopMatrix();
}

void dsInnerHouse::RenderCubeMap(const Math::Vec3 & position, int face, StageCubemap * render)
{
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    if (cbRT->Enable(face))
    {
        dev->ClearColor(0, 0, 0, 1);
        dev->Clear();
        dev->Viewport(0, 0, cbTex->GetWidth(), cbTex->GetHeight());

        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Perspective(1.0, 90, 0.01, 100);

        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();

        Math::Mat44 mv = Math::CubemapTransform(position, face);

        dev->LoadMatrix((float*)&mv);

        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();
    
        render->CubemapCapture(mv);

        cbRT->Disable();
    }
}

void dsInnerHouse::CubemapCapture(const Math::Mat44 & mv) {
    RenderFromView(mv);
}

void dsInnerHouse::RenderCorridor(const Math::Mat44 & mv, float animateDoor) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    const float bFrameRate = 24.0;

    corridor->Get()->GetObject("Porta")->Play(animateDoor*bFrameRate+861);

    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_CULL_FACE);

    LampConfig corridorLamps = generateLampConfig(corridor, mv);
    for (int i = 0; i < mats_corridor.size(); i++) {
        Scene::Material * mat = mats_corridor[i];
        Graph::Program * prog = mat->GetProgram();
        bindLampConfigToProg(corridorLamps, prog);
    }

    corridor->Get()->Render();
}

void dsInnerHouse::UpdateCorridorTransform(Math::Vec4 distance, Math::Vec4 modifier) {
    for (int i = 0; i < mats_corridor.size(); i++) {
        Scene::Material * mat = mats_corridor[i];
        Graph::Program * prog = mat->GetProgram();
        prog->SetVariable4f("tr_modifier", modifier.GetX(), modifier.GetY(), modifier.GetZ(), modifier.GetW());
        prog->SetVariable4f("tr_distance", distance.GetX(), distance.GetY(), distance.GetZ(), distance.GetW());
    }
}