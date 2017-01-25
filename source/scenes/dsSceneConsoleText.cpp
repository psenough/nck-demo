#include "dsSceneConsoleText.h"
#include "../dsUtils.h"

dsSceneConsoleText::dsSceneConsoleText(DS::Data * data) : DS::Stage(data) {

}

dsSceneConsoleText::~dsSceneConsoleText() {

}

void dsSceneConsoleText::Load() {
    
}

void dsSceneConsoleText::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    const float height = 1920 * m_Data->GetHeight() / m_Data->GetWidth();
    dev->Ortho2D(1920, height, 0, 1);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    Graph::Texture * fontTex = m_Data->GetTexture("texture://tex2d_sans_serif.png");
    Gui::FontMap * fontMap = m_Data->GetFontMap("script://sans_serif.txt");

    int charsCount = speed * (time - start) / 1e6;

    float fontSize = 64;

    std::string tmp = text.substr(0, MIN(charsCount, text.length()));

    float len = fontMap->GetLength(fontSize, tmp, true);

    
    int secs = 10 * (time - start) / 1e6;
   
    if (secs % 2 == 0) {
        dev->Color(0, 0, 0, 255);
        dev->PushMatrix();
        dev->Translate(position.GetX() + len/2 + fontSize*0.5, position.GetY() - fontSize*0.5, 0);
        DS::RenderSquare(dev, fontSize*0.5, fontSize, false);
        dev->PopMatrix();
    }

    dev->Color(0, 0, 0);
    fontTex->Enable();
    fontMap->Draw(position.GetX(), position.GetY(), fontSize, tmp, true,Gui::FONT_ALIGNMENT_MIDDLE);
    fontTex->Disable();
}

void dsSceneConsoleText::SetText(const std::string & str, Math::Vec2 pos, float speed) {
    text = str;
    position = pos;
    this->speed = speed;
}
