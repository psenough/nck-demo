
#ifndef DS_INNER_HOUSE_H
#define DS_INNER_HOUSE_H

#include "../dsStage.h"

class dsInnerHouse : public DS::Stage , public Scene::Processor{
public:
    dsInnerHouse(DS::Data * data);
    virtual ~dsInnerHouse();
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
    void RenderCubeMap(const Math::Vec3 & position, int face);
    void RenderFromView(const Math::Mat44 & viewMatrix, bool renderMet);
    void bindLampConfigToProg(LampConfig & config, Graph::Program * prog);
    LampConfig & generateLampConfig(DS::Compound * c, Math::Mat44 viewMatrix);
    Scene::Object * findNearestCamera(int keyframe);
    void HandleFinish(BXON::Map * map, Scene::Compound * compound);
    DS::Compound * mainHouse;
    DS::Compound * corridor;
    DS::Compound * cameras;
    std::vector<Scene::Material*> mats_house;
    std::vector<Scene::Material*> mats_corridor;
    std::vector<std::pair<float, Scene::Object*> > camMarkers;
    Graph::RTManager * cbRT;
    Graph::TextureCubeMap * cbTex;
    Graph::Program * cbMatProgram;
    Scene::MCRenderer * mc_renderer;
    std::vector<Scene::MCSphereShape> mc_spheres;
};

#endif