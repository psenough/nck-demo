
/**
* NCtoolKit © 2007-2017 Luís F.Loureiro, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*
*/

#ifndef _NCK_DS_LOGO_H_
#define _NCK_DS_LOGO_H_

#include "../dsStage.h"

class dsSceneLogo : public DS::Stage{
public:
    dsSceneLogo(DS::Data * data);
    virtual ~dsSceneLogo();

    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);

private:
    DS::Compound * logo;
    Graph::Program * logoShader;
};

#endif
