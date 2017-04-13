
#ifndef DS_SCENE_OUTPUT_FILTER_H
#define DS_SCENE_OUTPUT_FILTER_H

#include "../dsStage.h"

class dsSceneOutputFilter : public DS::Stage {
public:
    dsSceneOutputFilter(DS::Data * data);
    virtual ~dsSceneOutputFilter();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
private:
    void Square(float w, float h, bool invert);

    Graph::Texture2D * renderTexture;
    Graph::RTManager * manager;

    Graph::Program * program;
    Graph::Program * bugfix;
    Graph::RTManager * accumManager;
    Graph::Texture2D * accumTexture;

    float brightness;
    float contrast;
    float saturation;
};

#endif