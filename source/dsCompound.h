
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_COMPOUND_H
#define DS_COMPOUND_H

#include "dsConfig.h"

_DS_BEGIN

class Compound {
public:
    Compound(Graph::Device * dev);
    ~Compound();
    void Load(const std::string & filename);
    int Reload();
    Scene::Compound_Basic * Get();
private:
    std::string m_Filename;
    int64_t m_LastModifed;
    Graph::Device * m_Device;
    Scene::Compound_Basic * m_Compound;
};

_DS_END

#endif