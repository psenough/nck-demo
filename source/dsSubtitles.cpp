
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsSubtitles.h"
#include "nckUtils.h"

Subtitle::Subtitle() {
	dev = NULL;
	fontMap = NULL;
	fontTex = NULL;
	size = 16.0f;
	color = Math::Color4ub(255, 255, 255, 255);
}

Subtitle::Subtitle(Graph::Device * dev, Graph::Texture * t, Gui::FontMap * f) {
	this->dev = dev;
	this->fontMap = f;
	this->fontTex = t;
	size = 16.0f;
	color = Math::Color4ub(255, 255, 255, 255);
}

void Subtitle::Render(float t, float start, float duration) {
	if (t >= start && t <= start + duration) {
		float fadeIn = MIN((t - start) * 2, 1.0);
		float fadeOut = MIN((start + duration - t) * 2, 1.0);

        dev->Color(0, 0, 0, 0.5 * color.GetA() * fadeIn * fadeOut);
        fontTex->Enable();
        fontMap->Draw(position.GetX() + 2, position.GetY() + 2, size, text, true, Gui::FONT_ALIGNMENT_MIDDLE);
        fontTex->Disable();

		dev->Color(color.GetR(), color.GetG(), color.GetB(), color.GetA() * fadeIn * fadeOut);
		fontTex->Enable();
		fontMap->Draw(position.GetX(), position.GetY(), size, text, true, Gui::FONT_ALIGNMENT_MIDDLE);
		fontTex->Disable();
	}
}

void Subtitle::SetPosition(const Math::Vec2 & pos) {
	position = pos;
}

Math::Vec2 Subtitle::GetPosition() const {
	return position;
}

void Subtitle::SetSize(float s) {
	size = s;
}

float Subtitle::GetSize() const {
	return size;
}

void Subtitle::Set(const std::string & text, float size, Math::Color4ub col) {
	this->text = text;
	this->size = size;
	color = col;
}
