
/**
* NCtoolKit © 2007-2017 Luís F.Loureiro, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*
*/

#include "dsSceneColorFade.h"

dsColorFade::dsColorFade(DS::Data * data) : DS::Stage(data){
   
}

dsColorFade::~dsColorFade() {

}

void dsColorFade::Load(){
    Graph::Device * dev = m_Data->GetGraphicsDevice();

   
}

void dsColorFade::Render(int64_t start, int64_t end, int64_t time){
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    
    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());

    // Set the view (camera) matrix.
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    // Set the model matrix.
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
    
    float a = (time - start) / (float)(end - start);
    a = Math::Clamp(0, 1, a);

    Math::Color4ub res = startColor * (1 - a) + endColor * a;

    m_Data->GetShapeRenderer()->Square(0, 0, m_Data->GetWidth(), m_Data->GetHeight(), res);
}

void dsColorFade::setColors(const Math::Color4ub & start, const Math::Color4ub & end) {
    startColor = start;
    endColor = end;
}