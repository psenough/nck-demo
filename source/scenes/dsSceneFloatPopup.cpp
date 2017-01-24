#include "dsSceneFloatPopup.h"
#include "../dsUtils.h"

dsSceneFloatPopup::dsSceneFloatPopup(DS::Data * data) : DS::Stage(data) {

}

dsSceneFloatPopup::~dsSceneFloatPopup() {

}

void dsSceneFloatPopup::Load() {
    
}

void dsSceneFloatPopup::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    float rel = m_Data->GetHeight() / m_Data->GetWidth();
    dev->Ortho2D(1920, 1920 * rel, 0, 1);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    float showAlpha = Math::Clamp(0,1,(time - start)/showDuration);
    float hideAlpha = Math::Clamp(0, 1, (end-time) / hideDuration);

    float alpha = showAlpha * hideAlpha;

    Math::Vec2 pos = Math::LinearInterpolation(showPosA, showPosB, alpha);

    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->PushMatrix();
    dev->Translate(pos.GetX(), pos.GetY(), 0);

    dev->Color(0, 0, 0, 20);
    DS::RenderSquare(dev, width, alpha*height, false);

    dev->Color(250, 250, 250, 240);
    dev->PushMatrix();
    dev->Translate(0, -8, 0);
    DS::RenderSquare(dev, width, 8, false);
    dev->PopMatrix();
    DS::RenderSquare(dev, 40, 16, false);
    DS::RenderSquare(dev, 8, alpha*height, false);

    dev->PopMatrix();
}

void dsSceneFloatPopup::SetAnimation(Math::Vec2 a, Math::Vec2 b, float show,float hide) {
    showPosA = a;
    showPosB = b;
    showDuration = show;
    hideDuration = hide;
}

void dsSceneFloatPopup::SetDimensions(float width, float height) {
    this->width = width;
    this->height = height;
}