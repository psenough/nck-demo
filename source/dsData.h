
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_DATA_H
#define DS_DATA_H

#include "dsCompound.h"

_DS_BEGIN

class ReloadResult {
public:
    ReloadResult() { total = 0; success = 0; failure = 0; }
    ReloadResult(int total, int success, int failure) {
        this->total = total; this->success = success; this->failure = failure;
    }
    int GetTotal() { return total; };
    int GetSuccess() { return success; };
    int GetFailure() { return failure; };
private:
    int total;
    int success;
    int failure;
};

class Data {
public:
    Data(Core::Window * wnd, Graph::Device * gDev);
    ~Data();

    ReloadResult ReloadTextures();
    ReloadResult ReloadCompounds();
    ReloadResult ReloadShaders();

    Graph::Device * GetGraphicsDevice();
    Gui::ShapeRenderer * GetShapeRenderer();
    Gui::WidgetRenderer * GetWidgetRenderer();

    Graph::Program * LoadProgram(const std::string & filename);
    Graph::Texture * LoadTexture(const std::string & filename);
    Compound * LoadCompound(const std::string & filename);
    Gui::FontMap * LoadFontMap(const std::string & filename);

    Graph::Program * GetProgram(const std::string & filename);
    Graph::Texture * GetTexture(const std::string & filename);
    Compound * GetCompound(const std::string & filename);
    Gui::FontMap * GetFontMap(const std::string & filename);
      
    float GetAspect();
    float GetHeight();
    float GetWidth();
private:
    Core::Window * window;
    Graph::Device * gDevice;
    Gui::WidgetRenderer * widgetRenderer;
    Gui::ShapeRenderer * shapeRenderer;
    std::map<std::string, Gui::FontMap*> m_FontMap;
    std::map<std::string, Compound*> m_Compounds;
    std::map<std::string, Graph::Program*> m_Programs;
    std::map<std::string, Graph::Texture*> m_Textures;
};

_DS_END

#endif