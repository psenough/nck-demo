
#ifndef DS_SCENE_MAP_H
#define DS_SCENE_MAP_H

#include "../dsStage.h"

class BezNode {
public:
    BezNode(const Math::Vec3 & l, const Math::Vec3 & c, const Math::Vec3 & r);

    Math::Vec3 left;
    Math::Vec3 center;
    Math::Vec3 right;

    static float getDistance(const BezNode & a, const BezNode & b);
    static Math::Vec3 getValue(const BezNode & a, const BezNode & b, float t);
};

class BezCurve {
public:
    BezCurve();
    void Set(std::vector<BezNode> n);
    Math::Vec3 Get(float alpha);
    std::vector<BezNode> nodes;
    std::vector<float> distance;
    float totalDistance;
};

class dsSceneMap : public DS::Stage, public Scene::Processor {
public:
    dsSceneMap(DS::Data * data);
    virtual ~dsSceneMap();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
private:
    Scene::Texture * HandleTexture(Scene::Texture * tex);
    void HandleFinish(BXON::Map * map, Scene::Compound * compound);
    DS::Compound * map;
    Graph::Program * mapProg;
    BezCurve curve;
};

#endif