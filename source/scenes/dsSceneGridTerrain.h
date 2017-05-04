#ifndef DS_SCENE_GRID_TERRAIN_H
#define DS_SCENE_GRID_TERRAIN_H

#include "../dsStage.h"

class dsSceneGridTerrain : public DS::Stage {
public:
    dsSceneGridTerrain(DS::Data * data);
    virtual ~dsSceneGridTerrain();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

private:
    DS::Compound * terrainModel;
    Graph::Program * terrainProgram;
};

#endif