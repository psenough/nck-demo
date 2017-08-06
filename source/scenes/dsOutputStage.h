
#ifndef DS_OUTPUT_STAGE_H
#define DS_OUTPUT_STAGE_H

#include "../dsStage.h"

class dsOutputStage : public DS::Stage {
public:
    dsOutputStage(DS::Data * data);
    virtual ~dsOutputStage();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
protected:
    Scene::Object * findNearestCamera(int keyframe);
    Graph::Texture2D * fboTexture;
    Graph::RTManager * fboManager;
    Graph::Program * aaProgram;
};

#endif