#ifndef DS_SCENE_SYS_SHUTDOWN_H
#define DS_SCENE_SYS_SHUTDOWN_H

#include "../dsStage.h"

class dsSceneSysShutdown : public DS::Stage {
public:
    dsSceneSysShutdown(DS::Data * data);
    virtual ~dsSceneSysShutdown();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

    void RenderStars();

private:
    DS::Compound * sysLogo;
    Graph::Program * builder;
    DS::Compound * brain;
};

#endif