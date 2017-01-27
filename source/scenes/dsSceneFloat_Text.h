
#ifndef DS_SCENE_FLOAT_TEXT_H
#define DS_SCENE_FLOAT_TEXT_H

#include "dsSceneFloatPopup.h"

class dsSceneFloat_Text : public dsSceneFloatPopup {
public:
    dsSceneFloat_Text(DS::Data * data);
    virtual ~dsSceneFloat_Text();
    void Load();
    void setType(int user);
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
private:
    std::vector<std::string> usage;
};

#endif