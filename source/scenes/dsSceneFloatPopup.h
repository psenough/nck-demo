#ifndef DS_SCENE_FLOAT_POPUP_H
#define DS_SCENE_FLOAT_POPUP_H

#include "../dsStage.h"

class dsSceneFloatPopup : public DS::Stage {
public:
    dsSceneFloatPopup(DS::Data * data);
    virtual ~dsSceneFloatPopup();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

    void SetAnimation(Math::Vec2 a, Math::Vec2 b, float show,float hide);


    void SetDimensions(float width, float height);
protected:
    float width, height;
    Math::Vec2 showPosA;
    Math::Vec2 showPosB;
    float showDuration;

    Math::Vec2 hidePosA;
    Math::Vec2 hidePosB;
    float hideDuration;
private:
};


#endif