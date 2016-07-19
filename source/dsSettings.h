
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_SETTINGS_H
#define DS_SETTINGS_H

#include "dsConfig.h"

_DS_BEGIN

class DemoSettings : public Gui::CallbackHandler {
public:
	DemoSettings();
	~DemoSettings();
	void Load();
	void Render();
	bool Run(int * width, int * height, bool * fullscreen);
	void OnButtonFocus(Gui::Widget * widget);
	void OnButtonDown(Gui::Widget * widget);
	void OnButtonClick(Gui::Widget * widget);
	void OnButtonUp(Gui::Widget * widget);
	void OnValueChanged(Gui::Widget * widget);

	bool enterDemo;
	bool runFlag;

	std::vector< std::pair<int, int> > resolution;

	Gui::CheckBoxButton * fullScreenButton;
	Gui::PushButton * runButton;
	Gui::PushButton * exitButton;
	Gui::RangeOptionButton * resolutionSelector;

	Core::Window * wnd;
	Graph::Device * dev;
	Gui::Manager * wgtManager;
	Graph::Texture * fontTexture;
	Gui::FontMap * fontMap;
	Gui::ShapeRenderer * shpRender;
	Gui::WidgetRenderer * wgtRender;
};

#endif

_DS_END