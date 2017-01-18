
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsStage.h"

_DS_BEGIN

void Stage::AddStage(int64_t start, int64_t end, Stage * stage) {
    m_Stages.Insert(Math::TimelineItem<DS::Stage*>(start, end, stage));
}
void Stage::BuildTimeline() {
    m_Stages.Build();
}

_DS_END