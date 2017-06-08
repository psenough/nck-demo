
#include "dsInsideSatellite.h"

dsInsideSatellite::dsInsideSatellite(DS::Data * data) : DS::Stage(data) {
    uiFBOManager = NULL;
    uiFBOTexture = NULL;
    satelliteScreen = NULL;
    satScreenProg = NULL;
}

dsInsideSatellite::~dsInsideSatellite() {
    SafeDelete(uiFBOManager);
    SafeDelete(uiFBOTexture);
    SafeDelete(satelliteScreen);
    SafeDelete(satScreenProg);
}

void dsInsideSatellite::Load() {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    uiFBOManager = dev->CreateRTManager(m_Data->GetWidth(), m_Data->GetHeight());
    uiFBOTexture = dynamic_cast<Graph::Texture2D*>(
        dev->CreateTexture(Graph::TEXTURE_2D, m_Data->GetWidth(), m_Data->GetHeight(), 0, Graph::FORMAT_RGBA_8B, true)
        );
    uiFBOManager->Attach(0, uiFBOTexture);
    /*uiFBOTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    uiFBOTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    uiFBOTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NONE);
    uiFBOTexture->SetAdressMode(Graph::ADRESS_CLAMP);*/

    satelliteScreen = m_Data->LoadCompound("model://satellite_screen.bxon");

    satScreenProg = m_Data->LoadProgram("shader://satScreen.cpp");

    satelliteScreen->Get()->GetMaterial("Screen")->SetProgram(satScreenProg);
    Scene::Texture * tex = new Scene::Texture(dev);
    tex->SetTexture(uiFBOTexture);
    satelliteScreen->Get()->GetMaterial("Screen")->SetTextureLayer(0, new Scene::TextureLayer(tex, NULL, dev));
}


void dsInsideSatellite::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();

    std::list<Math::TimelineItem<DS::Stage*>> items;
    m_Stages.Get(time, &items);
   
    uiFBOManager->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, uiFBOTexture->GetWidth(), uiFBOTexture->GetHeight());

        ListFor(Math::TimelineItem<DS::Stage*>, items, i) {
            i->GetObject()->Render(i->GetStart(), i->GetEnd(), time);
        }
    }
    uiFBOManager->Disable();
}


void dsInsideSatellite::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * const dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * const shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;

    dev->Viewport(0,0, width, height);

    Scene::Camera * cam = satelliteScreen->Get()->GetCamera("Camera");
    cam->SetAspect(width / height);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);
   
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    cam->Enable(Graph::MATRIX_VIEW);

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->Enable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_CULL_FACE);

    uiFBOTexture->Enable();
    satelliteScreen->Get()->Render();
    uiFBOTexture->Disable();
}
