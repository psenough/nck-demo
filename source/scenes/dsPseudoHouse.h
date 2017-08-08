
#ifndef DS_PSEUDO_HOUSE_H
#define DS_PSEUDO_HOUSE_H

#include "../dsStage.h"
#include "dsInnerHouse.h"

class dsPseudoHouse : public DS::Stage{
public:
    dsPseudoHouse(DS::Data * data, dsInnerHouse * innerHouse);
    virtual ~dsPseudoHouse();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
protected:
    class LampConfig {
    public:
        LampConfig() {};
        Math::Vec4 lamp_pos[8]; // position, energy
        Math::Vec4 lamp_dir[8]; // normal, type (0 - point, 1 - spot, 2 - sun, 3 -area) 
        Math::Vec4 lamp_color[8]; // rgb, distance
        Math::Vec4 lamp_params[8]; // ????
    };
    void RenderFromView(const Math::Mat44 & viewMatrix);
    void bindLampConfigToProg(LampConfig & config, Graph::Program * prog);
    LampConfig & generateLampConfig(DS::Compound * c, Math::Mat44 viewMatrix);
 
    DS::Compound * pseudo;
    DS::Compound * cameras;
    dsInnerHouse * innerHouse;
    std::vector<Scene::Material*> mats_pseudo;
};

#endif