
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsTimeline.h"
#include "scenes/dsOutputStage.h"
#include "scenes/dsInnerHouse.h"

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
    dsOutputStage * inside = new dsOutputStage(data);
    inside->Load();

    dsInnerHouse * house = new dsInnerHouse(data);
    house->Load();

    inside->AddStage(0, 180e6, house);
    //timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, house));

    timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, inside));
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
