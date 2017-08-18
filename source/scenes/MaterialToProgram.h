
#ifndef DS_MATERIAL_TO_PROGRAM_H
#define DS_MATERIAL_TO_PROGRAM_H

#include "../dsStage.h"

class MaterialToProgram {
public:
    MaterialToProgram(Graph::Device * dev);
    ~MaterialToProgram();

    Graph::Program * generate(Scene::Material * mat);

    void SetEnableTransform(bool flag) { enableTransform = flag; }

    void SetEnableArmature(bool flag) { enableArmature = flag; }
private:
    Graph::Device * m_Device;
    bool enableTransform;
    bool enableArmature;
};

#endif