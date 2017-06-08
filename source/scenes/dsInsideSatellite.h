#ifndef DS_INSIDE_SATELLITE_H
#define DS_INSIDE_SATELLITE_H

#include "../dsStage.h"

class dsInsideSatellite : public DS::Stage {
public:
    dsInsideSatellite(DS::Data * data);
    virtual ~dsInsideSatellite();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
protected:
    Graph::Texture2D * uiFBOTexture;
    Graph::RTManager * uiFBOManager;
    DS::Compound * satelliteScreen;
    Graph::Program * satScreenProg;
};

#endif
