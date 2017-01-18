
#include "dsSceneMap.h"
#include "nckCamera.h"

#include <algorithm>

BezNode::BezNode(const Math::Vec3 & l, const Math::Vec3 & c, const Math::Vec3 & r) {
    left = l;
    center = c;
    right = r;
}

float BezNode::getDistance(const BezNode & a, const BezNode & b) {
    return (b.center - a.center).Length();
}

Math::Vec3 BezNode::getValue(const BezNode & a, const BezNode & b, float t) {
    return Math::BezierInterpolation(a.center, a.right, b.left, b.center, t);
}

BezCurve::BezCurve() {

}

void BezCurve::Set(std::vector<BezNode> n) {
    nodes = n;
    distance.clear();
    distance.reserve(nodes.size() - 1);
    totalDistance = 0;
    int resolution = 10;
    for (int i = 0; i < nodes.size()-1; i++) {
        BezNode c = nodes[i];
        BezNode n = nodes[i+1];
        Math::Vec3 last = c.center;
        float dist = 0.0;
        for (int j = 1; j < resolution; j++) {
            Math::Vec3 p = BezNode::getValue(c, n, j / (float)resolution);
            float d = (p - last).Length();
            dist += d;
            last = p;
        }
        distance.push_back(dist);
        totalDistance += dist;
    }
}

Math::Vec3 BezCurve::Get(float alpha) {
    float t = alpha * totalDistance;
    float dt = 0.0;
    if(alpha < 0.0)
        return Math::Vec3(nodes[0].center);

    for (int i = 0; i < nodes.size()-1; i++) {
        BezNode c = nodes[i];
        BezNode n = nodes[i + 1];
        if (t >= dt && t < (dt + distance[i]) ){
            float a = (t - dt)/ distance[i];
            return BezNode::getValue(c, n, a);
        }
        dt += distance[i];
    }
    return Math::Vec3(nodes[nodes.size() - 1].center);
}

dsSceneMap::dsSceneMap(DS::Data * data) : DS::Stage(data) {
    
}

dsSceneMap::~dsSceneMap() {

}

void dsSceneMap::Load() {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    map = m_Data->LoadCompound("model://map.bxon", dynamic_cast<Scene::Processor*>(this));
    mapProg = m_Data->LoadProgram("shader://map/map.cpp");
    //normal = m_Data->LoadProgram("shader://normal.cpp");
}


void dsSceneMap::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    Scene::Camera * cam = dynamic_cast< Scene::Camera *>(map->Get()->GetDatablock(Scene::DATABLOCK_CAMERA, "Camera"));
 
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    cam->Enable(Graph::MATRIX_VIEW);
 
   
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
  
    Math::Vec3 p = curve.Get((time - start) / (double)(end - start));
    p += Math::Vec3(-0.41855, -1.5, 0)-cam->GetObject()->GetPosition();
    dev->Translate(-p.GetX(), -p.GetY(),0);


    //dev->Disable(Graph::STATE_CULL_FACE);
    //dev->Enable(Graph::STATE_BLEND);
    //dev->Enable(Graph::STATE_DEPTH_TEST);
    //dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    dev->Color(255, 255, 255, 255);

   
    mapProg->Enable();
    map->Get()->Render();
    mapProg->Disable();

    //normal->Enable();
    //normal->SetVariable1f("time", time / 1e6);
    //icosphere->Get()->Render();
    //normal->Disable();
}

void dsSceneMap::HandleFinish(BXON::Map * map, Scene::Compound * compound) {
    BXON::Array * arr = map->GetArray("curve");
    
   
    for (int i = 0; i < arr->GetSize(); i++) {
        BXON::Map * m = arr->GetMap(i);
        BXON::Array * splines = m->GetArray("splines");
        std::vector<BezNode> nodes;
        nodes.reserve(splines->GetSize());
        for (int j = 0; j < splines->GetSize(); j++) {
            BXON::Map * n = splines->GetMap(j);
            BXON::Array * left = n->GetArray("left");
            BXON::Array * center = n->GetArray("center");
            BXON::Array * right = n->GetArray("right");
            for (int k = 0; k < left->GetSize() / 3; k++) {
                float lx = left->GetFloat(k * 3 + 0);
                float ly = left->GetFloat(k * 3 + 1);
                float lz = left->GetFloat(k * 3 + 2);

                float cx = center->GetFloat(k * 3 + 0);
                float cy = center->GetFloat(k * 3 + 1);
                float cz = center->GetFloat(k * 3 + 2);

                float rx = right->GetFloat(k * 3 + 0);
                float ry = right->GetFloat(k * 3 + 1);
                float rz = right->GetFloat(k * 3 + 2);

                Math::Vec3 l = Math::Vec3(lx, ly, lz);
                Math::Vec3 c = Math::Vec3(cx, cy, cz);
                Math::Vec3 r = Math::Vec3(rx, ry, rz);

                nodes.push_back(BezNode(l,c,r));
            }
            //Math::Vec3 left = n->GetVec3("left");
            //Math::Vec3 center = n->GetVec3("center");
            //Math::Vec3 right = n->GetVec3("right");
            
        }

        curve.Set(nodes);
        return;
    }
}

