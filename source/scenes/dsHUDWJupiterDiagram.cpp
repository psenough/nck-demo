
#include "dsHUDWJupiterDiagram.h"


dsHUDWJupiterDiagram::dsHUDWJupiterDiagram(DS::Data * data) : dsHUDWindow(data) {
    diagram = NULL;
    patternGridProgram = NULL;
    solidMatProgram = NULL;
    //tex = NULL;
}

dsHUDWJupiterDiagram::~dsHUDWJupiterDiagram() {
    SafeDelete(diagram);
    SafeDelete(patternGridProgram);
    //SafeDelete(tex);
}

void dsHUDWJupiterDiagram::Load() {
    diagram = m_Data->LoadCompound("model://ui_planets.bxon");
    patternGridProgram = m_Data->LoadProgram("shader://side.cpp");
    solidMatProgram = m_Data->LoadProgram("shader://material_solid.cpp");
    //tex = m_Data->LoadTexture("texture://tex2d_patttern.png");


    diagram->Get()->GetMaterial("Jupiter")->SetProgram(patternGridProgram);

    diagram->Get()->GetMaterial("Border")->SetProgram(solidMatProgram);
    diagram->Get()->GetMaterial("Ring")->SetProgram(solidMatProgram);
    diagram->Get()->GetMaterial("Moons")->SetProgram(solidMatProgram);

    //tex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    //tex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);

}

void dsHUDWJupiterDiagram::Render(int64_t start, int64_t end, int64_t time){
    //dynamic_cast<dsHUDWindow*>(this)->Render(start, end, time);
    dsHUDWindow::Render(start, end, time);

    Graph::Device * dev = m_Data->GetGraphicsDevice();
  
    dev->Viewport(m_x, m_y, m_width, m_height);

    Scene::Camera * cam = diagram->Get()->GetCamera("Camera");
    cam->SetAspect(m_width / m_height);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    cam->Enable(Graph::MATRIX_VIEW);


    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->Disable(Graph::STATE_CULL_FACE);

    dev->Color(255, 255, 255);

    const float t = (time - start) / 1.0e6;

    Scene::Object * moons[] = {
        diagram->Get()->GetObject("M1"),
        diagram->Get()->GetObject("M2"),
        diagram->Get()->GetObject("M3"),
        diagram->Get()->GetObject("M4")
    };

    for (int i = 0; i < 4; i++) {
        float dist = moons[i]->GetPosition().Length();
        moons[i]->SetPosition(Math::Vec3(cos(t+i), sin(t+i), 0)*dist);
        moons[i]->Update();
    }

    //tex->Enable();
    //side->Enable();
    diagram->Get()->Render();
   /// side->Disable();
    //tex->Disable();
}
