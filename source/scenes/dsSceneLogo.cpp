
/**
* NCtoolKit © 2007-2017 Luís F.Loureiro, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*
*/

#include "dsSceneLogo.h"

dsSceneLogo::dsSceneLogo(DS::Data * data) : DS::Stage(data){
   
}

dsSceneLogo::~dsSceneLogo() {

}

void dsSceneLogo::Load(){
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    logo = m_Data->LoadCompound("model://coprocessor-logo.bxon");
  
    logoShader = m_Data->LoadProgram("shader://logo.cpp");
}

void dsSceneLogo::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    float dt = (time - start) / 1e6;

    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->Disable(Graph::STATE_CULL_FACE);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());

    // Set the view (camera) matrix.
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    // Set the model matrix.
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    logoShader->Enable();
    logoShader->SetVariable1f("dt", dt);
    dev->Color(0, 255, 180, 200);
    dev->PushMatrix();
    dev->Translate(m_Data->GetWidth() / 2, m_Data->GetHeight() / 2, 0);
    dev->Scale(m_Data->GetHeight() / 3, -m_Data->GetHeight() / 3, 1);
    logo->Get()->Render();
    dev->PopMatrix();
    logoShader->Disable();
}

void dsSceneLogo::Render(int64_t start, int64_t end, int64_t time){
    RenderFBO(start, end, time);
}
