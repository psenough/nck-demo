
#include "dsLamps.h"

LampConfig LampConfig::generate(DS::Compound * c, Math::Mat44 viewMatrix) {
    std::vector<Scene::Object*> objects;
    c->Get()->GetObjectsWithLayer(&objects, Scene::DATABLOCK_LAMP);

    LampConfig r;

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


LampConfig LampConfig::generate(Math::Mat44 viewMatrix, std::vector<Scene::Object*> lampsObjs) {
    LampConfig r;

    for (size_t i = 0; i < lampsObjs.size() && i < 8; i++) {
        Scene::Object * obj = lampsObjs[i];
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

    if (lampsObjs.size() < 8)
        r.lamp_pos[lampsObjs.size()] = Math::Vec4(0, 0, 0, -1); // early terminator.

    return r;
}

void LampConfig::bind(LampConfig & config, Graph::Program * prog) {
    prog->SetArray4f("lamp_pos_mv", 8, (float*)config.lamp_pos);
    prog->SetArray4f("lamp_dir_mv", 8, (float*)config.lamp_dir);
    prog->SetArray4f("lamp_color", 8, (float*)config.lamp_color);
    prog->SetArray4f("lamp_params", 8, (float*)config.lamp_params);
}

/*
LampConfig LampConfig::operator=(LampConfig src) {
    for (int i = 0; i < 8; i++) {
        lamp_pos[i] = src.lamp_pos[i];
        lamp_dir[i] = src.lamp_dir[i];
        lamp_color[i] = src.lamp_color[i];
        lamp_params[i] = src.lamp_params[i];
    }
    return *this;
}*/