
#ifndef DS_SCENE_SCRATCH_H
#define DS_SCENE_SCRATCH_H

#include "../dsStage.h"

class dsSceneScratch : public DS::Stage {
public:
    dsSceneScratch(DS::Data * data);
    virtual ~dsSceneScratch();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
private:
    DS::Compound * s1;
    Graph::Program * p;
};

#endif