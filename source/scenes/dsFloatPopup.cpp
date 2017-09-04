#include "dsFloatPopup.h"
#include "../dsUtils.h"

dsFloatPopup::dsFloatPopup(DS::Data * data) : DS::Stage(data) {
   
}

dsFloatPopup::~dsFloatPopup() {

}

void dsFloatPopup::Load() {
  
}

void dsFloatPopup::RenderFBO(int64_t start, int64_t end, int64_t time) {

}

void dsFloatPopup::Update(int64_t start, int64_t end, int64_t time) {
    const float t = (time - start) / 1.0e6;
    float a = t / ((end - start) / 1.0e6);

    currentPos = Math::LinearInterpolation(showPosA, showPosB, a);

    float showAlpha = Math::Clamp(0, 1, (time - start) / showDuration);
    float hideAlpha = Math::Clamp(0, 1, (end - time) / hideDuration);

    currentAlpha = showAlpha * hideAlpha;
}

void dsFloatPopup::Render(int64_t start, int64_t end, int64_t time) {

}

void dsFloatPopup::SetAnimation(Math::Vec2 a, Math::Vec2 b, float show,float hide) {
    showPosA = a;
    showPosB = b;
    showDuration = show;
    hideDuration = hide;
}

void dsFloatPopup::SetDimensions(float width, float height) {
    this->width = width;
    this->height = height;
}