
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_UTILS_H
#define DS_UTILS_H

#include "dsConfig.h"

_DS_BEGIN
void RenderLoading(Core::Window * wnd, Graph::Device * dev);

void RenderSquare(Graph::Device * dev, float w, float h, bool invert);

void RenderSquareMatrix(Graph::Device * dev, float width, float height);
void RenderCrossesMatrix(Graph::Device * dev, float width, float height);
_DS_END

#endif