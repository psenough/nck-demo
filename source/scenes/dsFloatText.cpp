
#include "dsFloatText.h"

dsFloatText::dsFloatText(DS::Data * data) : DS::Stage(data) {
 
}

dsFloatText::~dsFloatText() {
   
}

void dsFloatText::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

}


void dsFloatText::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    
}

void dsFloatText::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;
    float alpha = t / ((end-start) / 1.0e6);
   
    Graph::Texture * const fontTex = m_Data->GetTexture("texture://tex2d_sans_serif.png");
    Gui::FontMap * const fontMap = m_Data->GetFontMap("script://sans_serif.txt");

    dev->Viewport(0,0, width, height);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Disable(Graph::STATE_DEPTH_TEST);
    //dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_CULL_FACE);

    dev->Color(color.GetR(), color.GetG(), color.GetB());
    fontTex->Enable();
    fontMap->Draw(position.GetX(), position.GetY(), fontSize, text, true, Gui::FONT_ALIGNMENT_MIDDLE);
    fontTex->Disable();
    dev->PopMatrix();
}

void dsFloatText::Set(const std::string & text, const Math::Vec2 & position, float fontSize, Math::Color4ub color) {
    this->text = text;
    this->position = position;
    this->fontSize = fontSize;
    this->color = color;
}