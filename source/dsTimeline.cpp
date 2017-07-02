
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsTimeline.h"

#include "scenes/dsSceneGridTerrain.h"
#include "scenes/dsHUDWindow.h"
#include "scenes/dsHUDWJupiterDiagram.h"
#include "scenes/dsInsideSatellite.h"
#include "scenes/dsOutputStage.h"
#include "scenes/dsSceneButterfly.h"
#include "scenes/dsSceneTest.h"

// Para correr sem audio usar NULL
#define AUDIO_STREAM        "audio://Ninja Tracks - Stasis.ogg" 
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

    //dsSceneGridTerrain * gt = new dsSceneGridTerrain(data);
    //gt->Load();
    //dsInsideSatellite * inside = new dsInsideSatellite(data);
    //inside->Load();
    
    dsOutputStage * inside = new dsOutputStage(data);
    inside->Load();

    dsSceneButterfly * test = new dsSceneButterfly(data);
    test->Load();

    dsSceneTest * tunel = new dsSceneTest(data);
    tunel->Load();

    inside->AddStage(0e6, 60e6, tunel);
    //

    inside->AddStage(1e6, 2e6, new StageProxy(test, 0));
    inside->AddStage(2e6, 3e6, new StageProxy(test, 0));
    inside->AddStage(3e6, 10e6, new StageProxy(test, 2e6));
    inside->AddStage(10e6, 11e6, new StageProxy(test, 4e6));
    inside->AddStage(11e6, 11.5e6, new StageProxy(test, 4e6));
    inside->AddStage(11.5e6, 12e6, new StageProxy(test, 4e6));
    inside->AddStage(12e6, 12.5e6, new StageProxy(test, 4e6));


    {
        auto w = data->GetWidth();
        auto h = data->GetHeight();

        auto l = w * 0.2;
        auto cw = w * 0.6;
        auto t = h * 0.1;

        dsHUDWindow * h1 = new dsHUDWindow(data);
        h1->Add(0.0, dsHUDWindow::Point(l, t, cw, 0, 0.0));
        h1->Add(0.3, dsHUDWindow::Point(l, t, cw, h * 0.2, 1.0));

        dsHUDWindow * h2 = new dsHUDWindow(data);
        h2->Add(1.0, dsHUDWindow::Point(l+cw*0.5, t + h * 0.2 + h*0.1, cw*0.5, 0, 0.0));
        h2->Add(1.3, dsHUDWindow::Point(l+cw*0.5, t + h * 0.2 + h*0.1, cw*0.5, h * 0.2, 1.0));
        h2->Add(10.0, dsHUDWindow::Point(l + cw*0.5, t + h * 0.2 + h*0.1, cw*0.5, h * 0.25, 1.0));


        dsHUDWindow * h3 = new dsHUDWindow(data);
        h3->Add(3.0, dsHUDWindow::Point(l + cw*0.5, t + h * 0.2 + 2*h*0.1 + h * 0.2, cw*0.5, 0, 0.0));
        h3->Add(3.3, dsHUDWindow::Point(l + cw*0.5, t + h * 0.2 + 2*h*0.1 + h * 0.2, cw*0.5, h * 0.2, 1.0));
        h3->Add(10.0, dsHUDWindow::Point(l + cw*0.5, t + h * 0.2 + 2 * h*0.1 + h * 0.2, cw*0.5, h * 0.25, 1.0));


        inside->AddStage(0e6, 60e6, new StageProxy(h1));
        inside->AddStage(0e6, 60e6, new StageProxy(h2));
        inside->AddStage(0e6, 60e6, new StageProxy(h3));

        dsHUDWJupiterDiagram * d = new dsHUDWJupiterDiagram(data);
        {
        float w2 = cw * 0.5 - cw*0.1;
        float h2 = w2 * 9 / 16.0;
        d->Load();
        d->Add(2.0, dsHUDWindow::Point(l, t + h * 0.2 + h*0.1, w2, 0, 0.0));
        d->Add(2.3, dsHUDWindow::Point(l, t + h * 0.2 + h*0.1, w2, h2, 1.0));
        inside->AddStage(0e6, 60e6, new StageProxy(d));
        }

    }

    timeline->Insert(Math::TimelineItem<DS::Stage*>(0e6, 60e6, inside));
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


