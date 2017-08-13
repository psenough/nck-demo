
#include "dsPseudoHouse.h"
#include "MaterialToProgram.h"

dsPseudoHouse::dsPseudoHouse(DS::Data * data, dsInnerHouse * innerHouse) : DS::Stage(data) {
    this->innerHouse = innerHouse;
}

dsPseudoHouse::~dsPseudoHouse() {
    
}

void dsPseudoHouse::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    pseudo = m_Data->LoadCompound("model://house_inner_3.bxon");
      
    pseudo->Get()->GetAllMaterials(&mats_pseudo);

    MaterialToProgram * mtp = new MaterialToProgram(m_Data->GetGraphicsDevice());
    
    std::vector<Scene::Texture*> t3;
    if (pseudo->Get()->GetAllTextures(&t3) > 0) {
        for (int i = 0; i < t3.size(); i++) {
            t3[i]->GetTexture()->SetAnisotropyFilter(8);
        }
    }
 
    for (int i = 0; i < mats_pseudo.size(); i++) {
        Graph::Program * p = mtp->generate(mats_pseudo[i]);
        mats_pseudo[i]->SetProgram(p);
    }

}

void dsPseudoHouse::Update(int64_t start, int64_t end, int64_t time) {
    innerHouse->Update(start, end, time);
}

void dsPseudoHouse::RenderFBO(int64_t start, int64_t end, int64_t time) {
    for (int i = 0; i < 6; i++)
        innerHouse->RenderCubeMap(Math::Vec3(0, 29.7094, 5.079279), i, this);
}

dsPseudoHouse::LampConfig & dsPseudoHouse::generateLampConfig(DS::Compound * c, Math::Mat44 viewMatrix) {
    std::vector<Scene::Object*> objects;
    c->Get()->GetObjectsWithLayer(&objects, Scene::DATABLOCK_LAMP);

    dsPseudoHouse::LampConfig r;

    for (size_t i = 0; i < objects.size() && i < 8; i++) {
        Scene::Object * obj = objects[i];
        Scene::Lamp * lamp = dynamic_cast<Scene::Lamp*>(obj->GetData());

        Math::Vec4 pMV = Math::Vec4(obj->GetPosition(), 1.0) * viewMatrix;
        Math::Vec4 dMV = Math::Vec4(0, 0, 0, 0);

        if (lamp->GetLampType() == Scene::LAMP_TYPE_SPOT || lamp->GetLampType() == Scene::LAMP_TYPE_SUN)
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

void dsPseudoHouse::bindLampConfigToProg(LampConfig & config, Graph::Program * prog) {
    prog->SetArray4f("lamp_pos_mv", 8, (float*)config.lamp_pos);
    prog->SetArray4f("lamp_dir_mv", 8, (float*)config.lamp_dir);
    prog->SetArray4f("lamp_color", 8, (float*)config.lamp_color);
    prog->SetArray4f("lamp_params", 8, (float*)config.lamp_params);
}

void dsPseudoHouse::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;

    Scene::Object * camObj = innerHouse->findNearestCamera(t * bFrameRate);

    if (camObj == NULL)
        return;

    Scene::Camera * cam = dynamic_cast<Scene::Camera*>(camObj->GetData());
    cam->SetAspect(aspect);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    
    cam->Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    Math::Mat44 viewMatrix = innerHouse->GetMainMV();
    dev->LoadMatrix((float*)&viewMatrix);

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    RenderFromView(viewMatrix);
    innerHouse->RenderMeta();
}

void dsPseudoHouse::RenderFromView(const Math::Mat44 & viewMatrix) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    dev->Enable(Graph::STATE_DEPTH_TEST);
    //dev->Enable(Graph::STATE_ZBUFFER_WRITE);
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_CULL_FACE);
    dev->CullMode(Graph::CULL_BACK);

    LampConfig houseLamps = generateLampConfig(pseudo, viewMatrix);
    for (int i = 0; i < mats_pseudo.size(); i++) {
        Scene::Material * mat = mats_pseudo[i];
        Graph::Program * prog = mat->GetProgram();
        bindLampConfigToProg(houseLamps, prog);
    }

    pseudo->Get()->Render();

    innerHouse->RenderButterfly();
}

void dsPseudoHouse::CubemapCapture(const Math::Mat44 & mv) {
    RenderFromView(mv);
}