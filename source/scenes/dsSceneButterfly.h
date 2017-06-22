#ifndef DS_SCENE_BUTTERFLY_H
#define DS_SCENE_BUTTERFLY_H

#include "../dsStage.h"

class dsSceneButterfly : public DS::Stage {
public:
    dsSceneButterfly(DS::Data * data);
    virtual ~dsSceneButterfly();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

private:
    DS::Compound * butterfly;
    Graph::Program * butterflyProgram;
};

#endif