#ifndef DS_SCENE_CONSOLE_TEXT_H
#define DS_SCENE_CONSOLE_TEXT_H

#include "../dsStage.h"

class dsSceneConsoleText : public DS::Stage {
public:
    dsSceneConsoleText(DS::Data * data);
    virtual ~dsSceneConsoleText();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

    void SetText(const std::string & str, Math::Vec2 pos, float speed);
protected:
    std::string text;
    float size;
    Math::Vec2 position;
    float speed;
private:
};


#endif