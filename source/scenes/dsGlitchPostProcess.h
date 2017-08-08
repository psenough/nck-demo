
#ifndef DS_GLITCH_POST_PROCESS_H
#define DS_GLITCH_POST_PROCESS_H

#include "../dsStage.h"

class dsGlitchPostProcess: public DS::Stage {
public:
    dsGlitchPostProcess(DS::Data * data);
    virtual ~dsGlitchPostProcess();
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