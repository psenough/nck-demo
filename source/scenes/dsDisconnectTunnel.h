
#ifndef DS_DISCONNECT_TUNNEL_H
#define DS_DISCONNECT_TUNNEL_H

#include "../dsStage.h"
#include "dsLamps.h"
#include "dsInnerHouse.h"

class dsDisconnectTunnel : public DS::Stage {
public:
    dsDisconnectTunnel(DS::Data * data);
    virtual ~dsDisconnectTunnel();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    void Update(int64_t start, int64_t end, int64_t time);

protected:
    DS::Compound * controller;
    DS::Compound * corredor;
    
    class CorridorPart {
        public:
            CorridorPart(Graph::Device * dev,DS::Compound * c, Scene::LayerMask layer);
            ~CorridorPart() {};

            LampConfig lighting;
            std::vector<Scene::Object*> lampsObjs;
            std::vector<Scene::Object*> modelsObjs;
            std::vector<Scene::Material*> mats;
            Scene::Object * door;
            Graph::Device * m_Device;
            void Render(const Math::Mat44 & mv, float anim, const Math::Vec4 & dist, const Math::Vec4 & modif);
    };

    std::vector<CorridorPart> blocks;
    std::vector<int> blocks_ids;
};

#endif