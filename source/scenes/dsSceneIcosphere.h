
#ifndef DS_SCENE_RT_H
#define DS_SCENE_RT_H

#include "../dsStage.h"

class dsSceneIcosphere : public DS::Stage, public Scene::Processor {
public:
    dsSceneIcosphere(DS::Data * data);
    virtual ~dsSceneIcosphere();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
private:
    Geometry::Mesh * HandleGeometry(Geometry::Mesh * mesh);
    DS::Compound * icosphere;
    Graph::Program * normal;
};

#endif