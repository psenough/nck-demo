
#ifndef DS_ROBOT_COLLAPSE_H
#define DS_ROBOT_COLLAPSE_H

#include "../dsStage.h"

class dsRobotCollapse : public DS::Stage {
public:
    dsRobotCollapse(DS::Data * data);
    virtual ~dsRobotCollapse();

    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void Update(int64_t start, int64_t end, int64_t time);
protected:
    DS::Compound * model;
    std::vector<Scene::Material*> mats;
};

#endif