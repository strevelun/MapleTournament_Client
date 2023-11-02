#include "UIManager.h"
#include "../Obj/UI/UI.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIEditText.h"

UIManager* UIManager::m_pInst = nullptr;

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    std::map<std::wstring, UI*>::iterator iter = m_mapUI.begin();
    std::map<std::wstring, UI*>::iterator iterEnd = m_mapUI.end();
    for (; iter != iterEnd; iter++)
        delete iter->second;
}


bool UIManager::AddUI(UI* _pUI)
{
    if (!_pUI) return false;

    const std::wstring& name = _pUI->GetName();
    if (FindUI(name) != nullptr)
        return false;
    m_mapUI.insert({ name, _pUI });
    m_vecUI.push_back(_pUI);
    return true;
}

void UIManager::AddPopupUI(UIPanel* _pUI)
{
    if (!_pUI) return;
    m_vecPopupUI.push_back(_pUI);
}

UI* UIManager::FindUI(const std::wstring& _strName)
{
    std::map<std::wstring, UI*>::iterator iter = m_mapUI.find(_strName);
    if (iter != m_mapUI.end())
        return iter->second;
    return nullptr;
}

bool UIManager::RemoveUI(const std::wstring& _strName)
{
    UI* pUI = FindUI(_strName);
    if (!pUI) return false;

    delete pUI;
    m_mapUI.erase(_strName);

    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();
    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->GetName() == _strName)
        {
            m_vecUI.erase(iter);
            return true;
        }
    }
    return false;
}

void UIManager::PopPopupUI()
{
    if (m_vecPopupUI.empty()) return;

    m_vecPopupUI.pop_back();
}

bool UIManager::Init()
{
    return true;
}

void UIManager::Update()
{
    if (!m_vecPopupUI.empty())
    {
        m_vecPopupUI.back()->Update();
        return;
    }

    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->IsActive())
            (*iter)->Update();
    }
}

void UIManager::Render()
{
    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->IsActive())
            (*iter)->Render();
    }

    std::vector<UIPanel*>::iterator iter2 = m_vecPopupUI.begin();
    std::vector<UIPanel*>::iterator iterEnd2 = m_vecPopupUI.end();

    for (; iter2 != iterEnd2; iter2++)
    {
        if ((*iter2)->IsActive())
            (*iter2)->Render();
    }
}

void UIManager::Cleanup()
{
    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();
    for (; iter != iterEnd; iter++)
    {
        delete *iter;
    }
    m_mapUI.clear();
    m_vecUI.clear();
    m_vecPopupUI.clear();
}
