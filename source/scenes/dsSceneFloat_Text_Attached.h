
#ifndef DS_SCENE_FLOAT_ATTACHED_H
#define DS_SCENE_FLOAT_ATTACHED_H

#include "dsSceneFloat_Text.h"
#include "dsSceneMap.h"

class dsSceneFloat_Text_Attached : public dsSceneFloat_Text {
public:
    dsSceneFloat_Text_Attached(DS::Data * data, dsSceneMap * map);
    virtual ~dsSceneFloat_Text_Attached();
    void setId(const std::string & id);
    void Render(int64_t start, int64_t end, int64_t time);
private:
    std::string id;
    dsSceneMap * map;
};

#endif