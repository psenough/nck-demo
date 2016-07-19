
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_SUBTITLES_H
#define DS_SUBTITLES_H

#include "dsConfig.h"

class Subtitle {
public:
	Subtitle();
	Subtitle(Graph::Device * dev, Graph::Texture * t, Gui::FontMap * f);
	void Render(float t, float start, float duration);
	void SetPosition(const Math::Vec2 & pos);
	Math::Vec2 GetPosition() const;
	void SetSize(float s);
	float GetSize() const;
	void Set(const std::string & text, float size = 16.0, Math::Color4ub col = Math::Color4ub(255,255,255));
private:
	Graph::Device * dev;
	Graph::Texture * fontTex;
	Gui::FontMap * fontMap;
	Math::Color4ub color;
	Math::Vec2 position;
	float size;
	std::string text;
};



#endif