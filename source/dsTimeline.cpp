
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsTimeline.h"
#include "scenes/dsOutputStage.h"
#include "scenes/dsInnerHouse.h"
#include "scenes/dsGlitchStage.h"

// Para correr sem audio usar NULL
#define AUDIO_STREAM        "audio://gwely_mernans.ogg" 
#define AUDIO_SAMPLERATE    44100
#define AUDIO_BUFFERS       4
#define AUDIO_FFT           2048

_DS_BEGIN

dsTimeline::dsTimeline(Core::Window * window) : dsMain(window) {

}

dsTimeline::~dsTimeline() {

}

void dsTimeline::LoadTimeline(Math::TimelineNode<DS::Stage*> * timeline) 
{    
    dsOutputStage * output = new dsOutputStage(data);
    output->Load();

    dsInnerHouse * house = new dsInnerHouse(data);
    house->Load();
    
    dsGlitchStage * glitch = new dsGlitchStage(data);
    glitch->Load();
    glitch->AddStage(0e6, 180e6, house);

   

    output->AddStage(0e6, 14e6, new StageProxy(house, 0));
    output->AddStage(14e6, 14.25e6, new StageProxy(glitch, 0));
    output->AddStage(14.25e6, 30e6, new StageProxy(house, 14.25e6));
    output->AddStage(30e6, 30.25e6, new StageProxy(glitch, 0));
    output->AddStage(30.25e6, 180e6, new StageProxy(house, 30.25e6));


    timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, new StageProxy(output,0)));
}

bool dsTimeline::LoadMusic(std::string * filename, int * sampleRate, int * buffers, int * fftSize) {
    if (AUDIO_STREAM != NULL) {
        *filename = AUDIO_STREAM;
        *sampleRate = AUDIO_SAMPLERATE;
        *buffers = AUDIO_BUFFERS;
        *fftSize = AUDIO_FFT;
        return true;
    }
    return false;
}

_DS_END
