
/**
* NCtoolKit © 2007-2017 Luís F.Loureiro, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*
*/

#ifndef _NCK_DS_COLOR_FADE_H_
#define _NCK_DS_COLOR_FADE_H_

#include "../dsStage.h"

class dsColorFade : public DS::Stage{
public:
    dsColorFade(DS::Data * data);
    virtual ~dsColorFade();

    void Load();
    void Render(int64_t start, int64_t end, int64_t time);

    void setColors(const Math::Color4ub & start, const Math::Color4ub & end);

private:
    Math::Color4ub startColor;
    Math::Color4ub endColor;
};

#endif
