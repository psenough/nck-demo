
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

    void AddStage(int64_t start, int64_t end, Stage * stage);
    void BuildTimeline();
    void SetOrder(int order) { m_Order = order; }
    int GetOrder() { return m_Order; }
protected:
    int m_Order;
    Data * m_Data;
    Math::TimelineNode<DS::Stage*> m_Stages;
};

_DS_END

#endif