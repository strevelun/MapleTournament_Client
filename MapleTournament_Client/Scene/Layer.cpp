#include "Layer.h"
#include "../Obj/Obj.h"

Layer::Layer(const std::wstring& _layerName, uint32_t _zOrder) :
    m_layerName(_layerName), m_zOrder(_zOrder)
{
}

Layer::~Layer()
{
}

void Layer::AddObj(Obj* _pObj)
{
    m_listObj.push_back(_pObj);
}

void Layer::Update()
{
    std::list<Obj*>::iterator iter = m_listObj.begin();
    std::list<Obj*>::iterator iterEnd = m_listObj.end();

    for (; iter != iterEnd; iter++)
    {
        (*iter)->Update();
    }
}

void Layer::Render(Graphics* _pGraphics)
{
    std::list<Obj*>::iterator iter = m_listObj.begin();
    std::list<Obj*>::iterator iterEnd = m_listObj.end();

    for (; iter != iterEnd; iter++)
    {
        (*iter)->Render(_pGraphics);
    }
}
