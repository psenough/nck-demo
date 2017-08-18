#ifndef DS_DISCONNECT_SCREEN_H
#define DS_DISCONNECT_SCREEN_H

#include "../dsStage.h"

class dsDisconnectScreen : public DS::Stage, public Scene::Processor {
public:
    dsDisconnectScreen(DS::Data * data);
    virtual ~dsDisconnectScreen();

    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void Update(int64_t start, int64_t end, int64_t time);
private:
    void Mutate();
    std::vector<Scene::Object*> refObj;
    std::vector<Scene::Object*> changeObj;
    DS::Compound * sysLogo;
    Graph::Program * fog;
};

#endif