
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsTimeline.h"
#include "scenes/dsOutputPostProcess.h"
#include "scenes/dsGlitchPostProcess.h"

#include "scenes/dsSolidColor.h"
#include "scenes/dsFloatText.h"

#include "scenes/dsInnerHouse.h"
#include "scenes/dsPseudoHouse.h"


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
    const float width = data->GetWidth();
    const float height = data->GetHeight();

    /*dsInnerHouse * house = new dsInnerHouse(data);
    dsPseudoHouse * pseudo = new dsPseudoHouse(data, house);

    pseudo->Load();
    house->Load();

    dsOutputPostProcess * output = new dsOutputPostProcess(data);
    output->Load();

    timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, new StageProxy(pseudo, 0)));
    */

    dsOutputPostProcess * output = new dsOutputPostProcess(data);
    dsInnerHouse * house = new dsInnerHouse(data);
    dsPseudoHouse * pseudo = new dsPseudoHouse(data,house);
    dsGlitchPostProcess * glitch = new dsGlitchPostProcess(data);

    renderLoading(20);
    output->Load();
    
    renderLoading(30);
    house->Load();

    renderLoading(60);
    pseudo->Load();

    renderLoading(80);
    glitch->Load();



    glitch->AddStage(0e6, 180e6, house);

   
    output->AddStage(0e6, 14e6, new StageProxy(house, 0));

    StageProxy * g1 = new StageProxy(glitch, 0);
    g1->SetRepeat(100000);
    output->AddStage(14e6, 15e6, g1);

    dsSolidColor * fadeIn0 = new dsSolidColor(data);
    fadeIn0->SetColors(Math::Color4ub(0, 0, 0, 255), Math::Color4ub(0, 0, 0, 0), true);
    StageProxy * fadeIn0P = new StageProxy(fadeIn0, 0);
    fadeIn0P->SetOrder(9999);
    output->AddStage(0, 5e6, fadeIn0P);
    
    output->AddStage(15e6, 30e6, new StageProxy(house, 14e6));


    StageProxy * g2 = new StageProxy(glitch, 0);
    g2->SetRepeat(100000);
    output->AddStage(30e6, 33e6,g2);

    dsFloatText * ftex1 = new dsFloatText(data);
    ftex1->Set("You are attempting to connect to\nan higher timebase than base reality", Math::Vec2(width*0.5, height*0.5), height*0.1);
    ftex1->SetOrder(9999);
    output->AddStage(30e6, 33e6, ftex1);
    

    output->AddStage(33e6, 34e6, new StageProxy(house, 30e6));

    output->AddStage(34e6, 36e6, new StageProxy(pseudo, 34e6));

    output->AddStage(36e6, 50e6, new StageProxy(house, 36e6));

  
        

    StageProxy * g3 = new StageProxy(glitch, 0);
    g3->SetRepeat(100000);
    output->AddStage(50e6, 53e6, g3);
    
    dsFloatText * ftex2 = new dsFloatText(data);
    ftex2->Set("Unable to meet clock requirements\ndisconnecting", Math::Vec2(width*0.5, height*0.5), height*0.1);
    ftex2->SetOrder(9999);
    output->AddStage(50e6, 53e6, ftex2);

    output->AddStage(53e6, 180e6, new StageProxy(house, 50e6));

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
