#include "Layer.h"
#include "../Obj/Obj.h"

Layer::Layer(const std::wstring& _layerName, uint32_t _zOrder) :
    m_layerName(_layerName), m_zOrder(_zOrder)
{
}

Layer::~Layer() // ObjectManager������
{
    std::vector<Obj*>::iterator iter = m_vecObj.begin();
    std::vector<Obj*>::iterator iterEnd = m_vecObj.end();

    for (; iter != iterEnd; ++iter)
        delete* iter;
    m_vecObj.clear();
}

void Layer::AddObj(Obj* _pObj)
{
    m_vecObj.push_back(_pObj);
}

void Layer::Update()
{
    std::vector<Obj*>::iterator iter = m_vecObj.begin();
    std::vector<Obj*>::iterator iterEnd = m_vecObj.end();

    for (; iter != m_vecObj.end();)
    {
        if (!(*iter)->IsAlive())
        {
            delete* iter;
            iter = m_vecObj.erase(iter);
        }
        else
        {
            (*iter)->Update();
            ++iter;
        }
    }
}

void Layer::Render()
{
    std::vector<Obj*>::iterator iter = m_vecObj.begin();
    std::vector<Obj*>::iterator iterEnd = m_vecObj.end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)->Render();
    }
}

bool Layer::CompareObjName(Obj* _pObj, const std::wstring& _strName)
{
    return _pObj->GetName() == _strName;
}
