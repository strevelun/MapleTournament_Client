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
    {
        delete iter->second;
    }
}


bool UIManager::AddUI(UI* _pUI)
{
    if (!_pUI) return false;
    const std::wstring& name = _pUI->GetName();
    if (FindUI(name) != nullptr)
        return false;
    m_mapUI.insert({ name, _pUI });
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

    std::map<std::wstring, UI*>::iterator iter = m_mapUI.begin();
    std::map<std::wstring, UI*>::iterator iterEnd = m_mapUI.end();

    for (; iter != iterEnd; iter++)
    {
        if ((iter->second)->IsActive())
            (iter->second)->Update();
    }
}

void UIManager::Render(Graphics* _pGraphics)
{
    std::map<std::wstring, UI*>::iterator iter = m_mapUI.begin();
    std::map<std::wstring, UI*>::iterator iterEnd = m_mapUI.end();

    for (; iter != iterEnd; iter++)
    {
        if ((iter->second)->IsActive())
            (iter->second)->Render(_pGraphics);
    }

    std::vector<UIPanel*>::iterator iter2 = m_vecPopupUI.begin();
    std::vector<UIPanel*>::iterator iterEnd2 = m_vecPopupUI.end();

    for (; iter2 != iterEnd2; iter2++)
    {
        if ((*iter2)->IsActive())
            (*iter2)->Render(_pGraphics);
    }
}

void UIManager::Cleanup()
{
    std::map<std::wstring, UI*>::iterator iter = m_mapUI.begin();
    std::map<std::wstring, UI*>::iterator iterEnd = m_mapUI.end();
    for (; iter != iterEnd; iter++)
    {
        delete iter->second;
    }
    m_mapUI.clear();
    m_vecPopupUI.clear();
}
