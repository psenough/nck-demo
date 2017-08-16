
#ifndef DS_ROBOT_COLLAPSE_H
#define DS_ROBOT_COLLAPSE_H

#include "../dsStage.h"

class dsRobotCollapse : public DS::Stage , public Scene::Processor {
public:
    dsRobotCollapse(DS::Data * data);
    virtual ~dsRobotCollapse();

    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void Update(int64_t start, int64_t end, int64_t time);
protected:
    void HandleFinish(BXON::Map * map, Scene::Compound * compound);

    std::vector<std::pair<float, Scene::Object*> > camMarkers;
    Scene::Object * findNearestCamera(int keyframe);
    DS::Compound * gynoid;
    std::vector<Scene::Material*> gynoidMats;

    DS::Compound * house;
    std::vector<Scene::Material*> houseMats;

    Math::Mat44 mainModelView;
    Math::Vec3 mainCamPos;
};

#endif