
#ifndef DS_CREDITS_H
#define DS_CREDITS_H

#include "../dsStage.h"

class dsCredits : public DS::Stage {
public:
    dsCredits(DS::Data * data);
    virtual ~dsCredits();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void Update(int64_t start, int64_t end, int64_t time);
protected:
    DS::Compound * model;
};

#endif