
#include "demoConfig.h"
#include "nckUtils.h"
#include "nckException.h"
#include "demoUtils.h"
#include "nckMathUtils.h"

DemoConfig::DemoConfig() {
	wnd = Core::CreateWindow("Configuration", 230 * Core::Window::GetDisplayDensity(), 70 * Core::Window::GetDisplayDensity(), false);
	runFlag = true;
	enterDemo = false;
	dev = NULL;
	fontMap = NULL;
	fontTexture = NULL;
	wgtManager = NULL;
	wgtRender = NULL;
	shpRender = NULL;

	resolution.reserve(9);
	resolution.push_back(std::pair<int, int>(640, 480));
	resolution.push_back(std::pair<int, int>(800, 600));
	resolution.push_back(std::pair<int, int>(1024, 768));
	resolution.push_back(std::pair<int, int>(1280, 720));
	resolution.push_back(std::pair<int, int>(1280, 768));
	resolution.push_back(std::pair<int, int>(1360, 768));
	resolution.push_back(std::pair<int, int>(1366, 768));
	resolution.push_back(std::pair<int, int>(1280, 1024));
	resolution.push_back(std::pair<int, int>(1920, 1080));
}

DemoConfig::~DemoConfig() {
	SafeDelete(wnd);
}

void DemoConfig::Load() {
	dev = Graph::CreateDevice(wnd, Graph::DEVICE_AUTO, wnd->GetWidth(), wnd->GetHeight());
	dev->ClearColor(100 / 255.0, 100 / 255.0, 100 / 255.0, 1);
	dev->ClearFlags(Graph::BUFFER_COLOR_BIT | Graph::BUFFER_DEPTH_BIT);

	dev->Enable(Graph::STATE_BLEND);
	dev->Enable(Graph::STATE_DEPTH_TEST);
	dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

	RenderLoading(wnd, dev);

	fontTexture = dev->LoadTexture("texture://tex2d_sans_serif.png");
	fontTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NEAREST);
	fontTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_LINEAR);
	fontTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_LINEAR);

	const float screenDensity = Core::Window::GetDisplayDensity();

	fontMap = new Gui::FontMap(dev, screenDensity);
	fontMap->Load("script://sans_serif.txt");
	fontMap->SetPositionAccuracy(Gui::FONT_POSITION_ACCURACY_INTEGER);


	shpRender = new Gui::ShapeRenderer(dev, screenDensity);
	wgtRender = new Gui::BlenderWidgetRenderer(dev, shpRender, fontMap, fontTexture);
	wgtManager = new Gui::Manager(0, 0, wnd->GetWidth(), wnd->GetHeight(), wgtRender);

	exitButton = new Gui::PushButton(10, 40, 100, 20, "Exit");
	exitButton->SetTooltip("Exit demo");
	exitButton->SetCallback(this);
	wgtManager->Add(exitButton);

	runButton = new Gui::PushButton(120, 40, 100, 20, "Run");
	runButton->SetTooltip("Run demo");
	runButton->SetCallback(this);
	wgtManager->Add(runButton);

	std::vector< std::pair<int, std::string> > roptValues;
	roptValues.reserve(9);
	for (int i = 0; i<resolution.size(); i++) {
		roptValues.push_back(std::pair<int, std::string>(i, Math::IntToString(resolution[i].first) + " x " + Math::IntToString(resolution[i].second)));
	}

	resolutionSelector = new Gui::RangeOptionButton(120, 10, 100, 20, roptValues);
	resolutionSelector->SetSelectedOption(3);
	resolutionSelector->SetTooltip("Screen Resolution Selector");
	resolutionSelector->SetCallback(this);
	wgtManager->Add(resolutionSelector);

	fullScreenButton = new Gui::CheckBoxButton(15, 13, 14, "Fullscreen");
	fullScreenButton->SetValue(true);
	wgtManager->Add(fullScreenButton);
}

void DemoConfig::Render() {
	dev->Clear();
	dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());

	dev->Disable(Graph::STATE_DEPTH_TEST);

	dev->MatrixMode(Graph::MATRIX_PROJECTION);
	dev->Identity();
	dev->Ortho2D(wnd->GetWidth(), wnd->GetHeight());

	dev->MatrixMode(Graph::MATRIX_MODEL);
	dev->Identity();

	wgtManager->Render();

	dev->PresentAll();
}

bool DemoConfig::Run(int * width, int * height, bool * fullscreen) {
	try {
		Load();
	}
	catch (const Core::Exception & ex) {
		Core::MsgBox("Ooops, an odd error was encountered, check exception.log in data and complain to @nzeroshift", "Error");
		ex.PrintStackTrace();
		SafeDelete(fontTexture);
		SafeDelete(fontMap);
		SafeDelete(wgtManager);
		SafeDelete(shpRender);
		SafeDelete(wgtRender);
		SafeDelete(dev);
		return false;
	}

	while (runFlag && wnd->Peek(true) && wnd->GetKeyStatus(Core::BUTTON_ESCAPE) == Core::BUTTON_STATUS_UP)
	{
		Render();

		Gui::UserInputState state;
		state.m_CursorX = wnd->GetMousePosition().GetX() / Core::Window::GetDisplayDensity();
		state.m_CursorY = wnd->GetMousePosition().GetY() / Core::Window::GetDisplayDensity();
		state.m_State_LMB = (wnd->GetMouseButtonStatus(Core::BUTTON_MOUSE_LEFT) != Core::BUTTON_STATUS_UP);

		wgtManager->Update(state);
	}
	int resIdx = resolutionSelector->GetSelectedOption();
	*width = resolution[resIdx].first;
	*height = resolution[resIdx].second;
	*fullscreen = fullScreenButton->GetValue();

	SafeDelete(fontTexture);
	SafeDelete(fontMap);
	SafeDelete(wgtManager);
	SafeDelete(wgtRender);
	SafeDelete(shpRender);
	SafeDelete(dev);

	return enterDemo;
}

void DemoConfig::OnButtonFocus(Gui::Widget * widget) {

}

void DemoConfig::OnButtonDown(Gui::Widget * widget) {

}

void DemoConfig::OnButtonClick(Gui::Widget * widget) {
	if (widget == runButton) {
		enterDemo = true;
		runFlag = false;
	}
	else if (widget == exitButton) {
		runFlag = false;
	}
}

void DemoConfig::OnButtonUp(Gui::Widget * widget) {

}

void DemoConfig::OnValueChanged(Gui::Widget * widget) {

}
