
#ifndef DS_FLOAT_USER_H
#define DS_FLOAT_USER_H

#include "dsFloatPopup.h"

class dsFloatTexture : public dsFloatPopup {
public:
    dsFloatTexture(DS::Data * data);
    virtual ~dsFloatTexture();
    
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
   
    void SetTexture(Graph::Texture * tex) { texture = tex; }
private:
    Graph::Texture * texture;
};

#endif