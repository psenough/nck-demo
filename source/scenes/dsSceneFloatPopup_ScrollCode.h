#ifndef DS_SCENE_FLOAT_POPUP_SC_H
#define DS_SCENE_FLOAT_POPUP_SC_H

#include "dsSceneFloatPopup.h"

class dsSceneFloatPopup_SC : public dsSceneFloatPopup {
public:
    dsSceneFloatPopup_SC(DS::Data * data);
    virtual ~dsSceneFloatPopup_SC();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
protected:
    Graph::Program * codeProg;
    Graph::Texture * codeTex;
private:
};


#endif