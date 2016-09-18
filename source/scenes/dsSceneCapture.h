
#ifndef DS_SCENE_RT_H
#define DS_SCENE_RT_H

#include "../dsStage.h"

class dsSceneCapture : public DS::Stage {
public:
    dsSceneCapture(DS::Data * data);
    virtual ~dsSceneCapture();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
private:
    Graph::RTManager * manager;
    Graph::Texture2D * texture;
};

#endif