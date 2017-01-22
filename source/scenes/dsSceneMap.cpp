
#include "dsSceneMap.h"
#include "nckCamera.h"
#include "../dsUtils.h"
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
    int resolution = 20;
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
    //depthRT = NULL;
    //depthTex = NULL;
    blur1RT = NULL;
    blur1Tex = NULL;
    blur2RT = NULL;
    blur2Tex = NULL;
    blurEffect = NULL;
    citiesObj.reserve(30);
}

dsSceneMap::~dsSceneMap() {
    //SafeDelete(depthRT);
    //SafeDelete(depthTex);

    SafeDelete(blur1RT);
    SafeDelete(blur1Tex);

    SafeDelete(blur2RT);
    SafeDelete(blur2Tex);

    SafeDelete(blurEffect);
}

void dsSceneMap::Load() {
    // Porto: 41.1580958,-8.6271923
    // Bratislava: 48.1468378,17.1090732
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    map = m_Data->LoadCompound("model://map.bxon", dynamic_cast<Scene::Processor*>(this));
    plane = m_Data->LoadCompound("model://plane.bxon");

    
    mapProg = m_Data->LoadProgram("shader://map/map.cpp");
    basic = m_Data->LoadProgram("shader://map/basic.cpp");
    depth = m_Data->LoadProgram("shader://map/depth.cpp");
    //ringProg = m_Data->LoadProgram("shader://map/plane_ring.cpp");

    //dynamic_cast<Scene::Material*>(plane->Get()->GetDatablock(Scene::DATABLOCK_MATERIAL, "Red"))->SetProgram(ringProg);
    dynamic_cast<Scene::Material*>(plane->Get()->GetDatablock(Scene::DATABLOCK_MATERIAL, "Metal"))->SetProgram(basic);

    int w = m_Data->GetWidth();
    int h = m_Data->GetHeight();
    //depthTex = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, w/2, h/2, 0, Graph::FORMAT_R_32F, true));
    //depthRT = dev->CreateRTManager(w / 2, h / 2);
    //depthRT->Attach(0, depthTex);

    blur1Tex = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, w, h, 0, Graph::FORMAT_RGBA_8B, true));
    blur1Tex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    blur1Tex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    blur1Tex->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NONE);
    blur1Tex->SetAdressMode(Graph::ADRESS_CLAMP);
    blur1RT = dev->CreateRTManager(w, h);
    blur1RT->Attach(0, blur1Tex);


    blur2Tex = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, w, h, 0, Graph::FORMAT_RGBA_8B, true));
    blur2Tex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    blur2Tex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    blur2Tex->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NONE);
    blur2Tex->SetAdressMode(Graph::ADRESS_CLAMP);
    blur2RT = dev->CreateRTManager(w, h);
    blur2RT->Attach(0, blur2Tex);

    blurEffect = Scene::Effect_DirectionalBlur::Create(dev, Scene::Effect_DirectionalBlur::WeightType::GAUSSIAN, 16);
    blurEffect->SetSize(Math::Vec2(w, h));
    blurEffect->SetDirection(Math::Vec2(0, 1));

   // invert = Scene::Effect_Invert::Create(dev);
}

void dsSceneMap::updateStuff(int64_t start, int64_t end, int64_t time) {
    float d_t = (time - start) / (double)(end - start);
    const Math::Vec3 curve_pos = curve.Get(d_t);

    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Scene::Camera * cam = dynamic_cast< Scene::Camera *>(map->Get()->GetDatablock(Scene::DATABLOCK_CAMERA, "Camera"));

    const Math::Mat44 proj = Math::Perspective(m_Data->GetWidth() / (float)m_Data->GetHeight(), 36.0, 0.1, 100.0);
    projectionMatrix = proj;
      
    const Math::Vec3 camOffset = Math::Vec3(curve_pos) - cam->GetObject()->GetPosition() + Math::Vec3(0, -0.5, 0);
    const Math::Mat44 view = Math::Translate(Math::Vec3(-camOffset.GetX(), -camOffset.GetY(), 0)) * cam->GetMatrix();
    viewMatrix = view;

    Math::Vec3 p1 = curve.Get(d_t - 0.001);
    Math::Vec3 p2 = curve.Get(d_t + 0.001);

    Math::Vec3 n = Math::Normalize(p2 - p1);
    float angle = acos(Math::Dot(n, Math::Vec3(0, -1, 0)));
    if (Math::Cross(n, Math::Vec3(0, 1, 0)).GetZ() < 0)
        angle = -angle;

    //Math::Mat44 matRot = Math::LookAt(p2, p1, Math::Vec3(0, 0, 1));
    Math::Quat q = Math::Rotation(Math::RotateZ(angle));

    Scene::Object * obj = dynamic_cast<Scene::Object*>(plane->Get()->GetDatablock(Scene::DATABLOCK_OBJECT, "Plane"));
    obj->SetPosition(curve_pos + Math::Vec3(0, 0, 0.05));
    obj->SetRotation(q);
    obj->Update();
}

void dsSceneMap::RenderFBO(int64_t start, int64_t end, int64_t time) {
    updateStuff(start, end, time);
    Graph::Device * dev = m_Data->GetGraphicsDevice();

   // ringProg->SetVariable1f("time", time / 1e6);

   

    blur1RT->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, blur1Tex->GetWidth(), blur1Tex->GetHeight());

        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->LoadMatrix((float*)&projectionMatrix);

        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();
        dev->LoadMatrix((float*)&viewMatrix);

        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        dev->Enable(Graph::STATE_BLEND);
        dev->Enable(Graph::STATE_DEPTH_TEST);

        mapProg->Enable();
        map->Get()->Render();
        mapProg->Disable();
     
        plane->Get()->Render();

        dev->Disable(Graph::STATE_DEPTH_TEST);
        dev->MatrixMode(Graph::MATRIX_PROJECTION); dev->Identity();
        dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());
        dev->MatrixMode(Graph::MATRIX_VIEW); dev->Identity();
        dev->MatrixMode(Graph::MATRIX_MODEL); dev->Identity();

        for (int i = 0; i < citiesObj.size(); i++) {
            Scene::Object * obj = citiesObj[i];
            Math::Vec3 p = obj->GetPosition();
            p.SetZ(0);
            Math::Vec4 proj_pos = Math::Vec4(p, 1.0) * viewMatrix * projectionMatrix;
            float ox = (proj_pos.GetX() / proj_pos.GetW() / 2 + 0.5) * m_Data->GetWidth();
            float oy = (-proj_pos.GetY() / proj_pos.GetW() / 2 + 0.5) * m_Data->GetHeight();

            dev->Color(200, 0, 0);
            m_Data->GetShapeRenderer()->Square(ox, oy, 10, 10, Math::Color4ub(255, 0, 0));
        }
    }
    blur1RT->Disable();

    blurEffect->SetDirection(Math::Vec2(1, 0));
    blur2RT->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, blur2Tex->GetWidth(), blur2Tex->GetHeight());

        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Ortho2D(blur2Tex->GetWidth(), blur2Tex->GetHeight());

        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();
 
        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        blurEffect->Enable();
        blur1Tex->Enable();
        DS::RenderSquare(dev, m_Data->GetWidth(), m_Data->GetHeight(), blur1RT->InvertedY());
        blur1Tex->Disable();
        blurEffect->Disable();
    }
    blur2RT->Disable();

    blurEffect->SetDirection(Math::Vec2(0, 1));
    blur1RT->Enable();
    {
        dev->Clear();
        dev->Viewport(0, 0, blur1Tex->GetWidth(), blur1Tex->GetHeight());

        dev->MatrixMode(Graph::MATRIX_PROJECTION);
        dev->Identity();
        dev->Ortho2D(blur1Tex->GetWidth(), blur1Tex->GetHeight());

        dev->MatrixMode(Graph::MATRIX_VIEW);
        dev->Identity();

        dev->MatrixMode(Graph::MATRIX_MODEL);
        dev->Identity();

        blurEffect->Enable();
        blur2Tex->Enable();
        DS::RenderSquare(dev, m_Data->GetWidth(), m_Data->GetHeight(), blur2RT->InvertedY());
        blur2Tex->Disable();
        blurEffect->Disable();
    }
    blur1RT->Disable();
}


void dsSceneMap::Render(int64_t start, int64_t end, int64_t time) {
    updateStuff(start, end, time);

    Graph::Device * dev = m_Data->GetGraphicsDevice();

    
    Scene::Object * tp_plane = dynamic_cast<Scene::Object*>(plane->Get()->GetDatablock(Scene::DATABLOCK_OBJECT, "Plane"));

    const Math::Vec4 proj_pos = Math::Vec4(tp_plane->GetPosition(), 1.0) * viewMatrix * projectionMatrix;
    float x = (proj_pos.GetX() / proj_pos.GetW() / 2 + 0.5) * m_Data->GetWidth();
    float y = (-proj_pos.GetY() / proj_pos.GetW() / 2 + 0.5) * m_Data->GetHeight();

 

  

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Disable(Graph::STATE_DEPTH_TEST);
  
    dev->Color(255, 255, 255, 255);
    blur1Tex->Enable();
    DS::RenderSquare(dev,m_Data->GetWidth(), m_Data->GetHeight(), blur1RT->InvertedY());
    blur1Tex->Disable();

    //

    Graph::Texture * fontTex = m_Data->GetTexture("texture://tex2d_sans_serif.png");
    Gui::FontMap * fontMap = m_Data->GetFontMap("script://sans_serif.txt");

    m_Data->GetShapeRenderer()->Square(x+50, y-13, 100, 26, Math::Color4ub(0, 0, 0, 50));
    fontTex->Enable();
    dev->Color(255, 255, 255);
    fontMap->Draw(x+50, y, 24, "P" + Math::FloatToString(tp_plane->GetPosition().GetX(),3) + " " + Math::FloatToString(tp_plane->GetPosition().GetY(), 3), true, Gui::FontAlignment::FONT_ALIGNMENT_LEFT);
    fontTex->Disable();


    /*dev->Color(255, 255, 255, 200);
    depthTex->Enable();
    DS::RenderSquare(dev, m_Data->GetWidth(), m_Data->GetHeight(), depthRT->InvertedY());
    depthTex->Disable();*/
}

Scene::Texture * dsSceneMap::HandleTexture(Scene::Texture * tex) {
    tex->GetTexture()->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    tex->GetTexture()->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    return tex;
}

Scene::Object * dsSceneMap::HandleObject(Scene::Object * obj) {
    if (obj->GetData() == NULL) {
        citiesObj.push_back(obj);
    }
    return obj;
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
        }

        curve.Set(nodes);
        return;
    }
}

