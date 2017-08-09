
#ifndef DS_OUTPUT_POST_PROCESS_H
#define DS_OUTPUT_POST_PROCESS_H

#include "../dsStage.h"

class dsOutputPostProcess : public DS::Stage {
public:
    dsOutputPostProcess(DS::Data * data);
    virtual ~dsOutputPostProcess();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    virtual void RenderFBO(int64_t start, int64_t end, int64_t time);
    virtual void Update(int64_t start, int64_t end, int64_t time);
protected:
    Graph::Texture2D * fboTexture;
    Graph::RTManager * fboManager;
    Graph::Program * aaProgram;
};

#endif