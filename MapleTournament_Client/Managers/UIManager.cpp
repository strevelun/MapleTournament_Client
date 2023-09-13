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
    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();
    for (; iter != iterEnd; iter++)
    {
        delete* iter;
    }
}


void UIManager::AddUI(UI* _pUI)
{
    if (!_pUI) return;
    m_vecUI.push_back(_pUI);
}

void UIManager::AddPopupUI(UIPanel* _pUI)
{
    if (!_pUI) return;
    m_vecPopupUI.push_back(_pUI);
}

UI* UIManager::FindUI(const std::wstring& _strName)
{
    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->GetName() == _strName)
            return *iter;
    }
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

    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->IsActive())
            (*iter)->Update();
    }
}

void UIManager::Render(Graphics* _pGraphics)
{
    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->IsActive())
            (*iter)->Render(_pGraphics);
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
    std::vector<UI*>::iterator iter = m_vecUI.begin();
    std::vector<UI*>::iterator iterEnd = m_vecUI.end();
    for (; iter != iterEnd; iter++)
    {
        delete* iter;
    }
    m_vecUI.clear();
    m_vecPopupUI.clear();
}
