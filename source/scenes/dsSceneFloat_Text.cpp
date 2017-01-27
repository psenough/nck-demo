
#include "dsSceneFloat_Text.h"
#include "nckCamera.h"
#include "../dsUtils.h"


dsSceneFloat_Text::dsSceneFloat_Text(DS::Data * data) : dsSceneFloatPopup(data){
    usage.reserve(10);
    setType(0);
}

dsSceneFloat_Text::~dsSceneFloat_Text() {
  
}

void dsSceneFloat_Text::Load() {
   
}


void dsSceneFloat_Text::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

}

void dsSceneFloat_Text::Render(int64_t start, int64_t end, int64_t time) {

    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    prepare(start, end, time);

    Math::Vec2 pos = Math::LinearInterpolation(showPosA, showPosB, alpha);

    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->PushMatrix();
    dev->Translate(pos.GetX(), pos.GetY(), 0);

    dev->Color(255, 255, 255, 240);
  
    DS::RenderSquare(dev, width, alpha * height, false);

    Graph::Texture * fontTex = m_Data->GetTexture("texture://tex2d_sans_serif.png");
    Gui::FontMap * fontMap = m_Data->GetFontMap("script://sans_serif.txt");

    float speed = 20.0;
    int charsCount = speed * (time - start) / 1e6;

    float fontSize = 64;

    std::string txt = usage[0];

    std::string tmp = txt.substr(0, MIN(charsCount, txt.length()));

    float len = fontMap->GetLength(fontSize, tmp, true);

    int secs = 10 * (time - start) / 1e6;

    Math::Vec2 position = Math::Vec2(10, alpha * height * 0.5 - fontSize / 2);

    if (secs % 2 == 0) {
        dev->Color(0, 0, 0, 255);
        dev->PushMatrix();
        dev->Translate(position.GetX() + len / 2 + fontSize*0.5, position.GetY() - fontSize*0.5, 0);
        DS::RenderSquare(dev, fontSize*0.5, fontSize, false);
        dev->PopMatrix();
    }

    dev->Color(0, 0, 0);
    fontTex->Enable();
    fontMap->Draw(position.GetX(), position.GetY(), fontSize, tmp, true, Gui::FONT_ALIGNMENT_LEFT);
    fontTex->Disable();

    drawBorders();
}

void dsSceneFloat_Text::setType(int usage) {
    if (usage == 0) {
        this->usage.push_back("jae686/code");
        this->usage.push_back("groups");
        this->usage.push_back(" -Volumetric Ilusions");
        this->usage.push_back(" -CoProcessor");
    }
    else if (usage == 1) {
        this->usage.push_back("zeroshift/code");
        this->usage.push_back("groups");
        this->usage.push_back(" -Napalm Core");
        this->usage.push_back(" -CoProcessor");
    }
    else if (usage == 2) {
        this->usage.push_back("ps/tpolm");
        this->usage.push_back("prev groups");
        this->usage.push_back(" -Napalm Core");
    }
}