
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

StageProxy::StageProxy(Data * data) : Stage(data) {
    m_Offset = 0;
    m_Stage = NULL;
}

StageProxy::StageProxy(Stage * stage, int64_t offset) : Stage(stage!=NULL?stage->GetData():NULL) {
    m_Stage = stage;
    m_Offset = offset;
}

StageProxy::~StageProxy() {

}

void StageProxy::Load() {

}

void StageProxy::Render(int64_t start, int64_t end, int64_t time) {
    if (m_Stage)
        m_Stage->Render(start, end, time + m_Offset);
}

void StageProxy::RenderFBO(int64_t start, int64_t end, int64_t time) {
    if (m_Stage)
        m_Stage->RenderFBO(start , end , time + m_Offset);
}

void StageProxy::SetOffset(int64_t offset) {
    m_Offset = offset;
}

void StageProxy::SetStage(Stage * stage) {
    m_Stage = stage;
}

_DS_END