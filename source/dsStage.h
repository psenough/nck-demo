
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_SCENE_H
#define DS_SCENE_H

#include "dsData.h"

_DS_BEGIN

class Stage
{
public:
    Stage(Data * data) { m_Data = data; m_Order = 0; };
    virtual ~Stage() {};
    virtual void Load() = 0;
    virtual void Render(int64_t start, int64_t end, int64_t time) = 0;
    virtual void RenderFBO(int64_t start, int64_t end, int64_t time) {};

    virtual void AddStage(int64_t start, int64_t end, Stage * stage);
    virtual void BuildTimeline();
    virtual void SetOrder(int order) { m_Order = order; }
    virtual int GetOrder() { return m_Order; }
    Data * GetData() { return m_Data; }
protected:
    int m_Order;
    Data * m_Data;
    Math::TimelineNode<DS::Stage*> m_Stages;
};

class StageProxy : public Stage{
public:
    StageProxy(Data * data);
    StageProxy(Stage * stage, int64_t offset = 0);
    ~StageProxy();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void SetOffset(int64_t offset);
    void SetStage(Stage * stage);
    void SetOrder(int order) { m_Stage->SetOrder(order); }
    int GetOrder() { return m_Stage->GetOrder(); }
    void SetRepeat(int64_t duration) { m_RepeatDuration = duration;}
    int64_t SetRepeat() { return m_RepeatDuration; }
protected:
    int64_t m_RepeatDuration;
    int64_t m_Offset;
    Stage * m_Stage;
};

_DS_END

#endif