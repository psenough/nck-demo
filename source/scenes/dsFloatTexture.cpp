
#include "dsFloatTexture.h"
#include "nckCamera.h"
#include "../dsUtils.h"

dsFloatTexture::dsFloatTexture(DS::Data * data) : dsFloatPopup(data){
    texture = NULL;
}

dsFloatTexture::~dsFloatTexture() {
  
}

void dsFloatTexture::Load() {
}



void dsFloatTexture::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

}

void dsFloatTexture::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    const float bFrameRate = 24.0;


    dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->PushMatrix();
    dev->Translate(currentPos.GetX(), currentPos.GetY(), 0);
    
    if (texture)
        texture->Enable();
    
    dev->Color(255, 255, 255, 255 * currentAlpha);
    DS::RenderSquare(dev, width, height, false);
    
    if (texture)
        texture->Disable();
    
    dev->PopMatrix();

    dev->Enable(Graph::STATE_ZBUFFER_WRITE);
    dev->Enable(Graph::STATE_DEPTH_TEST);
}