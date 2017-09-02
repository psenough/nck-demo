
#ifndef DS_SOLID_COLOR_H
#define DS_SOLID_COLOR_H

#include "../dsStage.h"

class dsSolidColor : public DS::Stage {
public:
    dsSolidColor(DS::Data * data);
    virtual ~dsSolidColor();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void SetColors(const Math::Color4ub & start, const Math::Color4ub & end, bool quadratic = false);
protected:
    Math::Color4ub startColor;
    Math::Color4ub endColor;
    bool quadratic;
};

#endif