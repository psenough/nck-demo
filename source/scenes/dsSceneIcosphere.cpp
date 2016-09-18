
#include "dsSceneIcosphere.h"
#include "nckCamera.h"

dsSceneIcosphere::dsSceneIcosphere(DS::Data * data) : DS::Stage(data) {
    
}

dsSceneIcosphere::~dsSceneIcosphere() {

}

void dsSceneIcosphere::Load() {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    icosphere = m_Data->LoadCompound("model://icosphere.bxon", dynamic_cast<Scene::Processor*>(this));
    normal = m_Data->LoadProgram("shader://normal.cpp");
}


void dsSceneIcosphere::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

  
    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Perspective(m_Data->GetAspect());

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
  
    dev->Enable(Graph::STATE_BLEND);
    dev->Enable(Graph::STATE_DEPTH_TEST);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    Scene::Camera camera(dev);
    Scene::Object obj(dev);
    obj.SetPosition(Math::Vec3(0, -8, 0));
    obj.SetRotation(Math::EulerToQuat(Math::Vec3(M_PI / 2, 0, 0)));
    camera.SetObject(&obj);

    camera.Enable(Graph::MATRIX_VIEW);

    normal->Enable();
    icosphere->Get()->Render();
    normal->Disable();
}


Geometry::Mesh * dsSceneIcosphere::HandleGeometry(Geometry::Mesh * mesh) {
    Geometry::VertexIterator vlast = mesh->m_Vertices.end();
    vlast--;
    mesh->m_UVLayers.push_back("layer");

    // Vexter Iterators are a bit odd, you can use temporary lists or the iterators won't be valid if the list 
    // is copied else where.
    int vCount = 0;

    
    ListFor(Geometry::Face*, mesh->m_Faces, f)
    {
        
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

           
        }     
        float rId = Math::RandomValue(0, mesh->m_Faces.size());
        std::vector<Math::Vec2> uvvec;
        uvvec.push_back(Math::Vec2((*f)->m_Id, rId));
        uvvec.push_back(Math::Vec2((*f)->m_Id, rId));
        uvvec.push_back(Math::Vec2((*f)->m_Id, rId));

        (*f)->m_UV.push_back(uvvec);

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