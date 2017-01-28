
#ifndef DS_SCENE_METABALL_H
#define DS_SCENE_METABALL_H

#include "dsSceneFloatPopup.h"

class dsSceneMetaballs : public dsSceneFloatPopup {
public:
    dsSceneMetaballs(DS::Data * data);
    virtual ~dsSceneMetaballs();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
private:
    //DS::Compound * gynoid;
    Graph::Texture2D * rtTexture;
    Graph::RTManager * rtManager;
    Graph::Program * basicProg;
    Graph::Program * materialProg;
    Scene::MCRenderer * processor;
    std::vector<Scene::MCSphereShape> spheres;
};

#endif