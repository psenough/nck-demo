#ifndef DS_SCENE_LOADING_H
#define DS_SCENE_LOADING_H

#include "../dsStage.h"

class dsSceneLoading : public DS::Stage {
public:
    dsSceneLoading(DS::Data * data);
    virtual ~dsSceneLoading();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

    void RenderStars();

private:
    Graph::Texture * logo;
};

void renderTips(Graph::Device * dev);

#endif