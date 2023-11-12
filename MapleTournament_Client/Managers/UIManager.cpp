#include "UIManager.h"
#include "../Obj/UI/UI.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIEditText.h"

#include <cassert>

UIManager* UIManager::m_pInst = nullptr;

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    std::map<std::wstring, UIPanel*>::iterator iter = m_mapPopupUI.begin();
    std::map<std::wstring, UIPanel*>::iterator iterEnd = m_mapPopupUI.end();
    for (; iter != iterEnd; ++iter)
        delete iter->second;
}


void UIManager::AddUI(UI* _pUI)
{
    if (!_pUI) return;

    const std::wstring& name = _pUI->GetName();
    if (FindUI(name) != nullptr)
        assert(false);
    m_mapUI.insert({ name, _pUI });
}

void UIManager::SetPopupUI(UIPanel* _pUI)
{
    if (!_pUI) return;

    _pUI->SetActive(true);
    _pUI->SetPopup(true);
    m_vecPopupUI.push_back(_pUI);
}

void UIManager::AddPopupUI(UIPanel* _pUI)
{
    if (!_pUI) return;

    const std::wstring& name = _pUI->GetName();
    if (FindUI(name) != nullptr)
        assert(false);
    m_mapPopupUI.insert({ name, _pUI });
}

UI* UIManager::FindUI(const std::wstring& _strName)
{
    std::map<std::wstring, UI*>::iterator iter = m_mapUI.find(_strName);
    if (iter != m_mapUI.end())
        return iter->second;

    std::map<std::wstring, UIPanel*>::iterator iter2 = m_mapPopupUI.find(_strName);
    if (iter2 != m_mapPopupUI.end())
        return iter2->second;

    return nullptr;
}

bool UIManager::RemoveUI(const std::wstring& _strName)
{
    UI* pUI = FindUI(_strName);
    if (!pUI) return false;

    delete pUI;
    m_mapUI.erase(_strName);

    return false;
}

void UIManager::PopPopupUI()
{
    if (m_vecPopupUI.empty()) return;

    m_vecPopupUI.back()->SetActive(false);
    m_vecPopupUI.back()->SetPopup(false);
    m_vecPopupUI.pop_back();
}

bool UIManager::PopupUIExist()
{
    return !m_vecPopupUI.empty();
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
    }
}

void UIManager::Render()
{
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
    std::map<std::wstring, UIPanel*>::iterator iter = m_mapPopupUI.begin();
    std::map<std::wstring, UIPanel*>::iterator iterEnd = m_mapPopupUI.end();
    for (; iter != iterEnd; ++iter)
        delete iter->second;
    
    m_mapUI.clear();
    m_mapPopupUI.clear();
    m_vecPopupUI.clear();
}
