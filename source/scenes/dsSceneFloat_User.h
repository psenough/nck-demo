
#ifndef DS_SCENE_FLOAT_USER_H
#define DS_SCENE_FLOAT_USER_H

#include "dsSceneFloatPopup.h"

class dsSceneFloat_User : public dsSceneFloatPopup {
public:
    dsSceneFloat_User(DS::Data * data);
    virtual ~dsSceneFloat_User();
    void Load();
    void setUser(int user);
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
private:
    int userId;
    Graph::Texture * userTex[3];
    Graph::Program * userProg;
};

#endif