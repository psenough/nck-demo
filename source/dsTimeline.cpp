
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsTimeline.h"
#include "scenes/dsOutputPostProcess.h"
#include "scenes/dsGlitchPostProcess.h"

#include "scenes/dsSolidColor.h"
#include "scenes/dsFloatText.h"
#include "scenes/dsConsoleText.h"

#include "scenes/dsInnerHouse.h"
#include "scenes/dsPseudoHouse.h"
#include "scenes/dsDisconnectTunnel.h"
#include "scenes/dsCredits.h"
#include "scenes/dsDisconnectScreen.h"
#include "scenes/dsRobotCollapse.h"
#include "scenes/dsFloatTexture.h"

// Comentar/Descomentar o audiostream desactiva/activa o suporte de audio.
#define AUDIO_STREAM        "audio://soundtrack.ogg" 
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
    dScreen->Load();

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
    output->AddStage(T_S(58.5), TM_S(1,35), new StageProxy(house, T_S(56.5)));

   

    // Disconnection Tunnel
    output->AddStage(TM_S(1, 27), TM_S(1, 57), new StageProxy(dTunnel, 0));

    //output->Load();
    //dScreen->Load();

    // Fade out para branco
    {
        dsSolidColor * fadeIn = new dsSolidColor(data);
        fadeIn->SetColors(Math::Color4ub(255, 255, 255, 0), Math::Color4ub(255, 255, 255, 255), true);
        fadeIn->SetOrder(9999);
        output->AddStage(TM_S(1, 53), TM_S(1, 57), fadeIn);
    }

    output->AddStage(TM_S(1, 57), TM_S(2, 7), dScreen);

    // Fade out para branco
    {
        dsSolidColor * fadeIn = new dsSolidColor(data);
        fadeIn->SetColors(Math::Color4ub(255, 255, 255, 255), Math::Color4ub(255, 255, 255, 0), true);
        fadeIn->SetOrder(9999);
        output->AddStage(TM_S(1, 57), TM_S(1, 58), fadeIn);
    }


    {
        dsConsoleText * txt1 = new dsConsoleText(data);
        txt1->SetText("Insufficient resources to maintain connection", Math::Vec2(0.5, 0.60), 20);
        output->AddStage(TM_S(1,57), TM_S(2, 2), txt1);
    }

    {
        dsConsoleText * txt1 = new dsConsoleText(data);
        txt1->SetText("Disconnecting...", Math::Vec2(0.5, 0.60), 20);
        output->AddStage(TM_S(2,2), TM_S(2,7), txt1);
    }

    // Fade out para preto
    {
        dsSolidColor * fadeIn = new dsSolidColor(data);
        fadeIn->SetColors(Math::Color4ub(0, 0, 0, 0), Math::Color4ub(0, 0, 0, 255), true);
        fadeIn->SetOrder(9999);
        output->AddStage(TM_S(2, 6.5), TM_S(2, 7), fadeIn);
    }

    //timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, new StageProxy(output)));



    output->AddStage(TM_S(2, 7), TM_S(2, 30), new StageProxy(robotHouse, 0));


    // Credits 
    {
        Math::Vec2 dim = Math::Vec2(width * 0.2, height * 0.2);
        Math::Vec2 pos = Math::Vec2(width, height) - dim;
    
        dsFloatTexture * ft1 = new dsFloatTexture(data);
        ft1->SetAnimation(pos, pos, 0.5, 0.5);
        ft1->SetDimensions(dim.GetX(), dim.GetY());
        
        ft1->SetTexture(data->LoadTexture("texture://tex2d_credits_1.png"));

        output->AddStage(TM_S(2, 7), TM_S(2, 11), ft1);
    }

    {
        Math::Vec2 dim = Math::Vec2(width * 0.2, height * 0.2);
        Math::Vec2 pos = Math::Vec2(width, height) - dim;

        dsFloatTexture * ft1 = new dsFloatTexture(data);
        ft1->SetAnimation(pos, pos, 0.5, 0.5);
        ft1->SetDimensions(dim.GetX(), dim.GetY());

        ft1->SetTexture(data->LoadTexture("texture://tex2d_credits_2.png"));

        output->AddStage(TM_S(2, 11), TM_S(2, 15), ft1);
    }

    {
        Math::Vec2 dim = Math::Vec2(width * 0.2, height * 0.2);
        Math::Vec2 pos = Math::Vec2(width, height) - dim;

        dsFloatTexture * ft1 = new dsFloatTexture(data);
        ft1->SetAnimation(pos, pos, 0.5, 0.5);
        ft1->SetDimensions(dim.GetX(), dim.GetY());

        ft1->SetTexture(data->LoadTexture("texture://tex2d_credits_3.png"));

        output->AddStage(TM_S(2, 15), TM_S(2, 19), ft1);
    }

    //output->AddStage(TM_S(2, 30), TM_S(2, 40), new StageProxy(credits, 0));




    timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 180e6, new StageProxy(output)));
    

    // Disconnection message
    /*{
    output->AddStage(TM_S(1, 10), TM_S(1, 14), new StageProxy(dScreen));

    dsFloatText * ftex1 = new dsFloatText(data);
    ftex1->Set("Disconnecting", Math::Vec2(width*0.5, height*0.5), height*0.1,Math::Color4ub(0,0,0));
    ftex1->SetOrder(9999);
    output->AddStage(TM_S(1, 10), TM_S(1, 14), ftex1);
    }*/
    //output->AddStage(TM_S(1,14), TM_S(1, 27), new StageProxy(house, T_S(68)));

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
