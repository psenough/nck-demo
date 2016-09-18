
#ifndef DS_SCENE_80GRID_H
#define DS_SCENE_80GRID_H

#include "../dsStage.h"

class dsScene80Grid : public DS::Stage {
public:
    dsScene80Grid(DS::Data * data);
    virtual ~dsScene80Grid();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
private:
    Graph::Program * grid;
    //Graph::Texture2D * circuit;
   // Graph::Texture2D * square;
};

#endif