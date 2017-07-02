#include "dsSceneTest.h"

dsSceneTest::dsSceneTest(DS::Data * data) : DS::Stage(data) {
    
}

dsSceneTest::~dsSceneTest() {
    
}

void dsSceneTest::Load() {
    tunel = m_Data->LoadCompound("model://tunel1.bxon");
    prog = m_Data->LoadProgram("shader://tunel.cpp");

    const int maxSections = 1000;
    const int maxSectionsTypes = 2;

    sectionsIds.reserve(maxSections);

    for (int i = 0; i < maxSections; i++) {
        sectionsIds.push_back((int)Math::RandomValue(0, maxSectionsTypes));
    }
}

void dsSceneTest::RenderSection(int partId) {
    if (partId == 0) {
        tunel->Get()->GetModel("T1")->Render();
    }
    if (partId == 1) {
        tunel->Get()->GetModel("T2")->Render();
    }
}

void dsSceneTest::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();
  
    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1e6;

    dev->Viewport(0, 0, width, height);

    Scene::Camera * cam = tunel->Get()->GetCamera("Camera");
    cam->SetAspect(width / height);

    cam->SetEnd(1000);
    //cam->GetObject()->Play(t * 24.0);

    
    dev->ClearColor(0.0, 0.0, 0.0, 1.0);
    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    cam->Enable(Graph::MATRIX_VIEW); 
  
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    float dim_y = 20.0;

    dev->Translate(sin(t), 0, cos(t));
    dev->Rotate(10 * sin(t), 0, 0, 1);

    dev->Enable(Graph::STATE_DEPTH_TEST);
    prog->SetVariable4f("modifier", cos(t), sin(t), 0, 0);

    prog->Enable();

    float sT = t * 20;

    float dt = sT - floor(sT / dim_y) * dim_y;
    int offset = (int)floor(sT / dim_y);

    for (int i = 0; i < 10; i++) {
        int modelIndex = sectionsIds[i + offset];

        dev->PushMatrix();
        //dev->Translate(0, i * dim_y, 0);
        prog->SetVariable4f("distance", 0.0, i * dim_y, dt);
        RenderSection(modelIndex);
        //tunel->Get()->Render();
        dev->PopMatrix();
        //
    }

    prog->Disable();
}