#ifndef DS_SCENE_TEST_H
#define DS_SCENE_TEST_H

#include "../dsStage.h"

class dsSceneTest : public DS::Stage {
public:
    dsSceneTest(DS::Data * data);
    virtual ~dsSceneTest();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

    void RenderSection(int partId);
private:
    std::vector<int> sectionsIds;
    DS::Compound * tunel;
    Graph::Program * prog;
};

#endif