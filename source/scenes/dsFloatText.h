#ifndef DS_SCENE_FLOAT_TEXT_H
#define DS_SCENE_FLOAT_TEXT_H

#include "../dsStage.h"

class dsFloatText : public DS::Stage {
public:
    dsFloatText(DS::Data * data);
    virtual ~dsFloatText();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void Set(const std::string & text, const Math::Vec2 & position, float fontSize = 24.0f, Math::Color4ub color = Math::Color4ub(255, 255, 255, 255));
private:
    Math::Vec2 position;
    float fontSize;
    Math::Color4ub color;
    std::string text;
};

#endif