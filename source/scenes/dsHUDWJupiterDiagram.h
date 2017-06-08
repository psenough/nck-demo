#ifndef DS_HUD_W_JUPITER_DIAGRAM_H
#define DS_HUD_W_JUPITER_DIAGRAM_H

#include "dsHUDWindow.h"

class dsHUDWJupiterDiagram : public dsHUDWindow {
public:
    dsHUDWJupiterDiagram(DS::Data * data);
    virtual ~dsHUDWJupiterDiagram();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
  
private:
    DS::Compound * diagram;
    Graph::Program * patternGridProgram;
    Graph::Program * solidMatProgram;
    //Graph::Texture * tex;
};

#endif