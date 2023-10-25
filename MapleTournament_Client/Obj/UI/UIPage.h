#pragma once
#include "UI.h"

#include <list>

class UIButton;
class UIList;
class UIPanel;

class UIPage :
    public UI
{
private:
    UIList* m_pUIList = nullptr;
    UIButton* m_pPrevBtn = nullptr, *m_pNextBtn = nullptr;

    u_int m_maxItemViewCount = 0;
    u_int m_curPage = 0;
    u_int m_maxPageCount = 0;

    ID2D1SolidColorBrush* m_pBrush = nullptr;

public:
    UIPage(const UIPage& _uiPage);
    UIPage(UI* _pParentPanel, UINT _width, UINT _height, UINT _itemWidth, UINT _itemHeight, INT _xpos = 0, INT _ypos = 0, FLOAT _pivotX = 0.f, FLOAT _pivotY = 0.f, float _itemVerticalInterval = 0.f);
    ~UIPage();

    UIPage* Clone() const override { return new UIPage(*this); }

    UIList* GetUIList() const { return m_pUIList; }
    u_int GetCurPageIdx() const { return m_curPage; }

    void SetCurPageIdx(u_int _idx) { m_curPage = _idx; }
    void SetItemTemplate(UIPanel* _pItem);
    void SetMaxItemViewCount(int _count) { m_maxItemViewCount = _count; }

    u_int GetMaxItemViewCount() const { return m_maxItemViewCount; }

    void Update() override;
    void Render() override;

    void Reset();
};

