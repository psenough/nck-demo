
#ifndef DS_GLITCH_STAGE_H
#define DS_GLITCH_STAGE_H

#include "../dsStage.h"

class dsGlitchStage : public DS::Stage {
public:
    dsGlitchStage(DS::Data * data);
    virtual ~dsGlitchStage();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
protected:
    Graph::Texture2D * noiseTexture;
    Graph::Texture2D * fboTexture;
    Graph::RTManager * fboManager;
    Graph::Program * aaProgram;
};

#endif