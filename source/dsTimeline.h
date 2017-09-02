
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsMain.h"

_DS_BEGIN

class dsTimeline : public dsMain
{
public:
    dsTimeline(Core::Window * window);
    virtual ~dsTimeline();

    void LoadTimeline(Math::TimelineNode<DS::Stage*> * timeline);
    bool LoadMusic(std::string * filename, int * sampleRate, int * buffers, int * fftSize);

    float GetMaxDuration();
};

_DS_END

