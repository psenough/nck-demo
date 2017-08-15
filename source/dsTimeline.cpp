
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
#include "scenes/dsDisconnectTunnel.h"
#include "scenes/dsCredits.h"
#include "scenes/dsDisconnectScreen.h"
#include "scenes/dsRobotCollapse.h"

// Comentar/Descomentar o audiostream desactiva/activa o suporte de audio.
//#define AUDIO_STREAM        "audio://jeenio1.ogg" 
#define AUDIO_SAMPLERATE    44100
#define AUDIO_BUFFERS       4
#define AUDIO_FFT           2048

#define T_S(s) s*1e6
#define TM_S(m,s) (m*60.0+s)*1e6

_DS_BEGIN

dsTimeline::dsTimeline(Core::Window * window) : dsMain(window) {

}

dsTimeline::~dsTimeline() {

}

void dsTimeline::LoadTimeline(Math::TimelineNode<DS::Stage*> * timeline) 
{    
    const float width = data->GetWidth();
    const float height = data->GetHeight();

    dsOutputPostProcess * output = new dsOutputPostProcess(data);
    dsInnerHouse * house = new dsInnerHouse(data);
    dsGlitchPostProcess * glitch = new dsGlitchPostProcess(data);
    dsDisconnectTunnel * dTunnel = new dsDisconnectTunnel(data);
    dsDisconnectScreen * dScreen = new dsDisconnectScreen(data);
    dsCredits * credits = new dsCredits(data);
    dsRobotCollapse * robotHouse = new dsRobotCollapse(data);

    //dsPseudoHouse * pseudo = new dsPseudoHouse(data,house);

    renderLoading(20);
    output->Load();
    glitch->Load();
    dScreen->Load();

    renderLoading(30);
    house->Load();

    renderLoading(60);   
    dTunnel->Load();

    renderLoading(80);
    robotHouse->Load();
    credits->Load();

    // Fade In
    {
        dsSolidColor * fadeIn = new dsSolidColor(data);
        fadeIn->SetColors(Math::Color4ub(0, 0, 0, 255), Math::Color4ub(0, 0, 0, 0), true);
        fadeIn->SetOrder(9999);
        output->AddStage(0, T_S(5), fadeIn);
    }

    // Presentation
    output->AddStage(0, T_S(56), new StageProxy(house, 0));
      
    // The glitch start
    {
        StageProxy * g1 = new StageProxy(house, T_S(56));
        g1->SetRepeat(T_S(0.2));
        output->AddStage(T_S(56), T_S(58), g1);
    }

    // Glitch finishes
    {
        glitch->AddStage(T_S(58), T_S(58.5), new StageProxy(house, T_S(56)));
        output->AddStage(T_S(58), T_S(58.5), new StageProxy(glitch, 0));
    }

    // Close door
    output->AddStage(T_S(58.5), TM_S(1,10), new StageProxy(house, T_S(56.5)));

    // Disconnection message
    {
        output->AddStage(TM_S(1, 10), TM_S(1, 14), new StageProxy(dScreen));

        dsFloatText * ftex1 = new dsFloatText(data);
        ftex1->Set("Disconnecting", Math::Vec2(width*0.5, height*0.5), height*0.1,Math::Color4ub(0,0,0));
        ftex1->SetOrder(9999);
        output->AddStage(TM_S(1, 10), TM_S(1, 14), ftex1);
    }

    output->AddStage(TM_S(1,14), TM_S(1, 27), new StageProxy(house, T_S(68)));


    // Disconnection Tunnel
    output->AddStage(TM_S(1, 27), TM_S(1, 57), new StageProxy(dTunnel, 0));


    output->AddStage(TM_S(1, 57), TM_S(2, 30), new StageProxy(robotHouse, 0));


    output->AddStage(TM_S(2, 30), TM_S(2, 40), new StageProxy(credits, 0));


    //pseudo->Load();
    //credits->Load();
    //dTunnel->Load();

    /*glitch->AddStage(0e6, 180e6, house);
        output->AddStage(0e6, 14e6, new StageProxy(house, 0));

    StageProxy * g1 = new StageProxy(glitch, 0);
    g1->SetRepeat(100000);
    output->AddStage(14e6, 15e6, g1);

   
    
    output->AddStage(15e6, 30e6, new StageProxy(house, 14e6));


    StageProxy * g2 = new StageProxy(glitch, 0);
    g2->SetRepeat(100000);
    output->AddStage(30e6, 33e6,g2);

    dsFloatText * ftex1 = new dsFloatText(data);
    ftex1->Set("You are attempting to connect to\nan higher timebase than base reality", Math::Vec2(width*0.5, height*0.5), height*0.1);
    ftex1->SetOrder(9999);
    output->AddStage(30e6, 33e6, ftex1);
    

    output->AddStage(33e6, 34e6, new StageProxy(house, 33e6));

    output->AddStage(34e6, 36e6, new StageProxy(pseudo, 34e6));

    output->AddStage(36e6, 43e6, new StageProxy(house, 36e6));

    StageProxy * g3 = new StageProxy(glitch, 0);
    g3->SetRepeat(100000);
    output->AddStage(43e6, 46e6, g3);
    
    dsFloatText * ftex2 = new dsFloatText(data);
    ftex2->Set("Unable to meet clock requirements\ndisconnecting", Math::Vec2(width*0.5, height*0.5), height*0.1);
    ftex2->SetOrder(9999);
    output->AddStage(43e6, 46e6, ftex2);
     
    //

    output->AddStage(80e6, 90e6, new StageProxy(credits, 0));

    // O post process é a track principal da timeline.
    timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, output));
    */

    /*dsOutputPostProcess * output = new dsOutputPostProcess(data);
    output->Load();

    dsDisconnectTunnel * dTunnel = new dsDisconnectTunnel(data, NULL);
    dsCredits * credits = new dsCredits(data);

    dTunnel->Load();
    credits->Load();*/

    //timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, new StageProxy(dTunnel)));

    //output->AddStage(0e6, 90e6, new StageProxy(dTunnel, 0));

    timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, new StageProxy(output)));

}

bool dsTimeline::LoadMusic(std::string * filename, int * sampleRate, int * buffers, int * fftSize) {
#ifdef AUDIO_STREAM
        *filename = AUDIO_STREAM;
        *sampleRate = AUDIO_SAMPLERATE;
        *buffers = AUDIO_BUFFERS;
        *fftSize = AUDIO_FFT;
        return true;
#else
    return false;
#endif
}

_DS_END
