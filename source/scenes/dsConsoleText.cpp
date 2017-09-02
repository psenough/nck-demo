
#include "dsConsoleText.h"
#include "../dsUtils.h"

dsConsoleText::dsConsoleText(DS::Data * data) : DS::Stage(data) {

}

dsConsoleText::~dsConsoleText() {

}

void dsConsoleText::Load() {
    
}

void dsConsoleText::RenderFBO(int64_t start, int64_t end, int64_t time) {

}

void dsConsoleText::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    Graph::Texture * fontTex = m_Data->GetTexture("texture://tex2d_sans_serif.png");
    Gui::FontMap * fontMap = m_Data->GetFontMap("script://sans_serif.txt");

    int charsCount = speed * (time - start) / 1e6;

    const float fontSize = 64.0 * (width / 1920.0);

    std::string tmp = text.substr(0, MIN(charsCount, text.length()));

    const float len = fontMap->GetLength(fontSize, tmp, true);

    const Math::Vec2 pos = position * Math::Vec2(width, height);
    
    int secs = 10 * (time - start) / 1e6;
   
    if (secs % 2 == 0) {
        dev->Color(0, 0, 0, 255);
        dev->PushMatrix();
        dev->Translate(pos.GetX() + len/2 + fontSize*0.5, pos.GetY() - fontSize*0.5, 0);
        DS::RenderSquare(dev, fontSize*0.5, fontSize, false);
        dev->PopMatrix();
    }

    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->Color(0, 0, 0);
    fontTex->Enable();
    fontMap->Draw(pos.GetX(), pos.GetY(), fontSize, tmp, true,Gui::FONT_ALIGNMENT_MIDDLE);
    fontTex->Disable();
}

void dsConsoleText::SetText(const std::string & str, Math::Vec2 pos, float speed) {
    text = str;
    position = pos;
    this->speed = speed;
}
