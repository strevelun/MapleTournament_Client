#include "Scene.h"
#include "Layer.h"
#include "../Managers/UIManager.h"

#include <algorithm>

Scene::Scene()
{
}

Scene::~Scene()
{
    std::vector<Layer*>::iterator iter = m_vecObjLayer.begin();
    std::vector<Layer*>::iterator iterEnd = m_vecObjLayer.end();

    for (; iter != iterEnd; ++iter)
        delete* iter;
    m_vecObjLayer.clear();
    UIManager::GetInst()->Cleanup();
}

Layer* Scene::FindLayer(const std::wstring& _layerName)
{
    std::vector<Layer*>::iterator iter = m_vecObjLayer.begin();
    std::vector<Layer*>::iterator iterEnd = m_vecObjLayer.end();

    for (; iter != iterEnd; ++iter)
    {
        if ((*iter)->GetName() == _layerName) return *iter;
    }

    return nullptr;
}

Layer* Scene::CreateLayer(const std::wstring& _layerName, uint32_t _zOrder, bool _isActive)
{
    Layer* pLayer = FindLayer(_layerName);
    if(!pLayer) pLayer = new Layer(_layerName, _zOrder, _isActive);

    m_vecObjLayer.push_back(pLayer);
    if (m_vecObjLayer.size() >= 2)
        std::sort(m_vecObjLayer.begin(), m_vecObjLayer.end(), Scene::SortLayer);

    return pLayer;
}

bool Scene::SortLayer(const Layer* a, const Layer* b)
{
    return a->GetZOrder() < b->GetZOrder();
}

bool Scene::Init()
{
    return true;
}

void Scene::Update()
{
    std::vector<Layer*>::iterator iter = m_vecObjLayer.begin();
    std::vector<Layer*>::iterator iterEnd = m_vecObjLayer.end();

    for (; iter != iterEnd; ++iter)
    {
        if((*iter)->IsActive())
            (*iter)->Update();
    }
}

void Scene::Render()
{
    std::vector<Layer*>::iterator iter = m_vecObjLayer.begin();
    std::vector<Layer*>::iterator iterEnd = m_vecObjLayer.end();

    for (; iter != iterEnd; ++iter)
        if ((*iter)->IsActive())
            (*iter)->Render();
}
