#ifndef DS_CONSOLE_TEXT_H
#define DS_CONSOLE_TEXT_H

#include "../dsStage.h"

class dsConsoleText : public DS::Stage {
public:
    dsConsoleText(DS::Data * data);
    virtual ~dsConsoleText();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void SetText(const std::string & str, Math::Vec2 pos, float speed);
protected:
    std::string text;
    float size;
    Math::Vec2 position;
    float speed;
private:
};


#endif