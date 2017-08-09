
#ifndef DS_INNER_HOUSE_H
#define DS_INNER_HOUSE_H

#include "../dsStage.h"

class StageCubemap {
public:
    virtual ~StageCubemap() {};
    virtual void CubemapCapture(const Math::Mat44 & mv) = 0;
};

class dsInnerHouse : public DS::Stage , public Scene::Processor, public StageCubemap {
public:
    dsInnerHouse(DS::Data * data);
    virtual ~dsInnerHouse();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void Update(int64_t start, int64_t end, int64_t time);

    void RenderButterfly();
    void RenderMeta();

    Scene::Object * findNearestCamera(int keyframe);
    void CubemapCapture(const Math::Mat44 & mv);
    void RenderCubeMap(const Math::Vec3 & position, int face, StageCubemap * render);
    void RenderCorridor(const Math::Mat44 & mv, float animateDoor);
    void UpdateCorridorTransform(Math::Vec4 distance, Math::Vec4 modifier);
    Math::Mat44 GetMainMV() {return mainModelView;};
    Math::Vec3 GetMainCamPos() { return mainCamPos; };
    Math::Vec3 GetMetaPos() { return metaPos; };

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
   
    void HandleFinish(BXON::Map * map, Scene::Compound * compound);
    DS::Compound * pseudo;
    DS::Compound * mainHouse;
    DS::Compound * corridor;
    DS::Compound * cameras;
    DS::Compound * butterfly;
    std::vector<Scene::Material*> mats_house;
    std::vector<Scene::Material*> mats_corridor;
    std::vector<Scene::Material*> mats_pseudo;
    std::vector<std::pair<float, Scene::Object*> > camMarkers;
    Graph::RTManager * cbRT;
    Graph::TextureCubeMap * cbTex;
    Graph::Program * cbMatProgram;
    Scene::MCRenderer * mc_renderer;
    std::vector<Scene::MCSphereShape> mc_spheres;
    //std::vector<RenderWhat> renderwhat;

    Math::Mat44 mainModelView;
    Math::Vec3 mainCamPos;
    Math::Vec3 metaPos;
};

#endif