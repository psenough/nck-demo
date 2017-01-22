
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsUtils.h"

_DS_BEGIN

void RenderLoading(Core::Window * wnd, Graph::Device * dev) {
	dev->Clear();
	dev->Disable(Graph::STATE_DEPTH_TEST);

	dev->MatrixMode(Graph::MATRIX_PROJECTION);
	dev->Identity();
	float sHeight = 1024 * wnd->GetHeight() / (float)wnd->GetWidth();
	dev->Ortho2D(1024, sHeight);

	dev->MatrixMode(Graph::MATRIX_MODEL);
	dev->Identity();

	int off = 200;
	int spacer = 40;
	for (int x = -off; x < 1024; x += spacer) {
		dev->Color(60, 60, 60);
		dev->Begin(Graph::PRIMITIVE_QUADS);
		dev->Vertex(x, 0);
		dev->Vertex(x + off, sHeight);
		dev->Vertex(x + off + spacer, sHeight);
		dev->Vertex(x + spacer, 0);
		dev->End();

		dev->Color(70, 70, 70);
		x += spacer;
		dev->Begin(Graph::PRIMITIVE_QUADS);
		dev->Vertex(x, 0);
		dev->Vertex(x + off, sHeight);
		dev->Vertex(x + off + spacer, sHeight);
		dev->Vertex(x + spacer, 0);
		dev->End();
	}
	dev->PresentAll();
}


void RenderSquare(Graph::Device * dev, float w, float h, bool invert) {
 
    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->TexCoord(0, invert ? 1 : 0);
    dev->Vertex(0, 0);
    dev->TexCoord(0, invert ? 0 : 1);
    dev->Vertex(0, h);
    dev->TexCoord(1, invert ? 0 : 1);
    dev->Vertex(w, h);
    dev->TexCoord(1, invert ? 1 : 0);
    dev->Vertex(w, 0);
    dev->End();
}

_DS_END