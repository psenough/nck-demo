#ifndef DS_SCENE_SYS_INIT_H
#define DS_SCENE_SYS_INIT_H

#include "../dsStage.h"

class dsSceneSysInit : public DS::Stage, public Scene::Processor {
public:
    dsSceneSysInit(DS::Data * data);
    virtual ~dsSceneSysInit();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

    void RenderStars();

private:
    Geometry::Mesh * HandleGeometry(Geometry::Mesh * mesh);
    Graph::Program * builder;
    DS::Compound * menger;
    DS::Compound * sysLogo;

    DS::Compound * brain;
    DS::Compound * cube;
    DS::Compound * sphere;
};

Geometry::Mesh * reworkToExplodeBuild(Geometry::Mesh * mesh);

#endif