
#ifndef DS_SCENE_80GRID_H
#define DS_SCENE_80GRID_H

#include "dsSceneFloatPopup.h"

class dsScene80Grid : public dsSceneFloatPopup {
public:
    dsScene80Grid(DS::Data * data);
    virtual ~dsScene80Grid();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
private:
    Graph::Texture2D * rtTexture;
    Graph::RTManager * rtManager;
    Graph::Program * grid;
};

#endif