
#include "dsSolidColor.h"

dsSolidColor::dsSolidColor(DS::Data * data) : DS::Stage(data) {
    quadratic = false;
}

dsSolidColor::~dsSolidColor() {
   
}

void dsSolidColor::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

}


void dsSolidColor::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    
}

void dsSolidColor::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    float alpha = t / ((end-start) / 1.0e6);
    
    if (quadratic)
        alpha *= alpha;

    //dev->Viewport(0,0, width, height);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_CULL_FACE);

    Math::Color4ub col = startColor * (1.0 - alpha) + endColor * alpha;

    m_Data->GetShapeRenderer()->Square(0, 0, width, height, col);

}

void dsSolidColor::SetColors(const Math::Color4ub & start, const Math::Color4ub & end, bool quadratic) {
    startColor = start;
    endColor = end;
}