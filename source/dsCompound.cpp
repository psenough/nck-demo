
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsCompound.h"

_DS_BEGIN

Compound::Compound(Graph::Device * dev) {
    m_Compound = NULL;
    m_Device = dev;
    processor = NULL;
}

Compound::~Compound(){
    SafeDelete(m_Compound);
}

void Compound::Load(const std::string & filename) {
    m_Filename = filename;
    Scene::Compound_Base * cb = new Scene::Compound_Base(m_Device);
    cb->Load(filename);
    m_LastModifed = Core::GetFileLastModified(filename);
    m_Compound = cb;
}

void Compound::Load(const std::string & filename, Scene::Processor * processor) {
    m_Filename = filename;
    Scene::Compound_Base * cb = new Scene::Compound_Base(m_Device);
    cb->Load(filename,processor);
    m_LastModifed = Core::GetFileLastModified(filename);
    m_Compound = cb;
}

int Compound::Reload() {
    int64_t lastModified = Core::GetFileLastModified(m_Filename);
    if (m_LastModifed != lastModified) {
        Scene::Compound_Base * cb = new Scene::Compound_Base(m_Device);
        try {
            cb->Load(m_Filename);
        }
        catch (const Core::Exception & e) {
            delete cb;
            THROW_EXCEPTION_STACK("Unable to load file",e);
        }
        m_LastModifed = lastModified;
        SafeDelete(m_Compound);
        m_Compound = cb;
        return 1;
    }
    return 0;
}

Scene::Compound_Base * Compound::Get() {
    return m_Compound;
}

_DS_END