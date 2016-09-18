
#include "dsSceneCapture.h"
#include "nckCamera.h"

dsSceneCapture::dsSceneCapture(DS::Data * data) : DS::Stage(data) {
    
}

dsSceneCapture::~dsSceneCapture() {

}

void dsSceneCapture::Load() {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    manager = dev->CreateRTManager(m_Data->GetWidth(), m_Data->GetHeight());
    texture = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, m_Data->GetWidth(), m_Data->GetHeight(), 0, Graph::FORMAT_RGBA_8B, true));
}


void dsSceneCapture::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

  

}