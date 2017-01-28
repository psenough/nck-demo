#include "dsSceneSystemInit.h"
#include "../dsUtils.h"
#include <algorithm>

dsSceneSysInit::dsSceneSysInit(DS::Data * data) : DS::Stage(data) {

}

dsSceneSysInit::~dsSceneSysInit() {

}

void dsSceneSysInit::Load() {
    sysLogo = m_Data->LoadCompound("model://logo_machine.bxon");

    builder = m_Data->LoadProgram("shader://builder.cpp");

    brain = m_Data->LoadCompound("model://brain.bxon", this);
    cube = m_Data->LoadCompound("model://cube.bxon", this);
    sphere = m_Data->LoadCompound("model://sphere.bxon", this);
}

void dsSceneSysInit::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();

    dev->ClearColor(0.0, 0.0, 0.0, 1.0);
  

    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);
    dev->Disable(Graph::STATE_CULL_FACE);
    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height, 0, 1);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    shp->Square(0, 0, width, height, Math::Color4ub(255, 255, 255));



    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Perspective(width / height);
  
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dynamic_cast<Scene::Camera*>(brain->Get()->GetDatablock(Scene::DATABLOCK_CAMERA, "Camera"))->Enable();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->FillMode(Graph::PolygonMode::FILL_WIREFRAME);
    dev->Color(0, 0, 0);
    //
    
    builder->Enable();
    //builder->SetVariable1f("time", (time-start) / 1e6);
    //menger->Get()->Render();
    //brain->Get()->Render();

    float f_time = (time - start) / 1e6;

    builder->SetVariable1f("rotate", 0.0);
    builder->SetVariable1f("failure", 1.0);
    if (f_time >= 0 && f_time < 13){
        builder->SetVariable1f("delay", 40);
        builder->SetVariable1f("time", f_time);
        cube->Get()->Render();
    }
    else if (f_time >= 13 && f_time < 26){
        builder->SetVariable1f("delay", 40);
        builder->SetVariable1f("time", f_time - 13);
        sphere->Get()->Render();
    }
    else {
        builder->SetVariable1f("rotate", (f_time-26)*0.1);
        builder->SetVariable1f("delay", 10);
        builder->SetVariable1f("failure", 0.0);
        builder->SetVariable1f("time", f_time - 26);
        brain->Get()->Render();
    }


    builder->Disable();
    dev->FillMode(Graph::PolygonMode::FILL_SOLID);



    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(width, height, 0, 1);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
      
    float scaleFactor = width / 1920;

    dev->PushMatrix();
    dev->Translate(width / 2, height / 2, 0);
    dev->Scale(300* scaleFactor,-300* scaleFactor,300* scaleFactor);
    dev->Color(200, 0, 0);
    sysLogo->Get()->Render();
    dev->PopMatrix();

    float alpha = Math::RandomValue(0, 20);
    dev->Color(30, 30, 30, alpha);
    DS::RenderCrossesMatrix(dev, m_Data->GetWidth(), m_Data->GetHeight());
}


Geometry::Mesh * dsSceneSysInit::HandleGeometry(Geometry::Mesh * mesh) {
    return  reworkToExplodeBuild(mesh);
}

Geometry::Mesh * reworkToExplodeBuild(Geometry::Mesh * mesh) {
    Geometry::VertexIterator vlast = mesh->m_Vertices.end();
    vlast--;

    mesh->m_UVLayers.push_back("layer");
    mesh->m_UVOptimization.push_back(false);

    mesh->m_UVLayers.push_back("center");
    mesh->m_UVOptimization.push_back(false);

    mesh->m_UVLayers.push_back("random");
    mesh->m_UVOptimization.push_back(false);

    int vCount = 0;

    std::vector<float> randomList;
    for (int i = 0; i < mesh->m_Faces.size(); i++) {
        randomList.push_back(i);
    }
    std::random_shuffle(randomList.begin(), randomList.end());

    int index = 0;
    ListFor(Geometry::Face*, mesh->m_Faces, f)
    {
        Math::Vec3 center;

        for (int vId = 0; vId < (*f)->m_Verts.size(); vId++)
        {
            Geometry::VertexIterator v = (*f)->m_Verts[vId];
            Geometry::Vertex * nv = new Geometry::Vertex();

            *nv = *(*v);
            nv->m_Id = vCount++;

            mesh->m_Vertices.push_back(nv);
            Geometry::VertexIterator vi = mesh->m_Vertices.end();
            vi--;

            (*f)->m_Verts[vId] = vi;

            center += nv->m_Pos;
        }

        center *= 1.0 / (*f)->m_Verts.size();

        float rId = randomList[index++];
        // randomList.pop_front();
        //std::vector<Math::Vec2> uvvec;
        //uvvec.push_back(Math::Vec2(rId, (*f)->m_Id));
        //uvvec.push_back(Math::Vec2(rId, (*f)->m_Id));
        //uvvec.push_back(Math::Vec2(rId, (*f)->m_Id));

        (*f)->m_fUV.SetUV(0, 0, rId, (*f)->m_Id);
        (*f)->m_fUV.SetUV(0, 1, rId, (*f)->m_Id);
        (*f)->m_fUV.SetUV(0, 2, rId, (*f)->m_Id);

        (*f)->m_fUV.SetUVZ(1, 0, center);
        (*f)->m_fUV.SetUVZ(1, 1, center);
        (*f)->m_fUV.SetUVZ(1, 2, center);

        Math::Vec3 randomDirection = Math::Normalize(Math::Vec3(Math::RandomValue(-1, 1), Math::RandomValue(-1, 1), Math::RandomValue(-1, 1)));
        (*f)->m_fUV.SetUVZ(2, 0, randomDirection);
        (*f)->m_fUV.SetUVZ(2, 1, randomDirection);
        (*f)->m_fUV.SetUVZ(2, 2, randomDirection);


        if ((*f)->m_Verts.size() == 3) {
            Math::Vec3 v1 = (*(*f)->m_Verts[0])->m_Pos;
            Math::Vec3 v2 = (*(*f)->m_Verts[1])->m_Pos;
            Math::Vec3 v3 = (*(*f)->m_Verts[2])->m_Pos;

            Math::Vec3 n = Math::Normalize(Math::Cross(Math::Normalize(v2 - v1), Math::Normalize(v3 - v1)));
            (*(*f)->m_Verts[0])->m_Nor = n;
            (*(*f)->m_Verts[1])->m_Nor = n;
            (*(*f)->m_Verts[2])->m_Nor = n;
        }
    }

    Geometry::VertexIterator popCurrent = mesh->m_Vertices.begin();
    while (true) {
        bool finish = popCurrent == vlast;
        delete (*mesh->m_Vertices.begin());
        popCurrent++;
        mesh->m_Vertices.pop_front();
        if (finish)
            break;
    }

    return mesh;
}