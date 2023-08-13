#include "Scene.h"
#include "Layer.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

Layer* Scene::FindLayer(const std::wstring& _layerName)
{
    std::list<Layer*>::const_iterator iter = m_listLayer.cbegin();
    std::list<Layer*>::const_iterator iterEnd = m_listLayer.cend();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->GetName() == _layerName) return *iter;
    }

    return nullptr;
}

Layer* Scene::CreateLayer(const std::wstring& _layerName, uint32_t _zOrder)
{
    Layer* pLayer = FindLayer(_layerName);
    if(!pLayer) pLayer = new Layer(_layerName, _zOrder);
    m_listLayer.push_back(pLayer);

    return pLayer;
}

bool Scene::Init()
{
    return true;
}

void Scene::Render(ID2D1HwndRenderTarget* _pRenderTarget)
{
    std::list<Layer*>::iterator iter = m_listLayer.begin();
    std::list<Layer*>::iterator iterEnd = m_listLayer.end();

    for (; iter != iterEnd; iter++)
    {
        (*iter)->Render(_pRenderTarget);
    }
}
