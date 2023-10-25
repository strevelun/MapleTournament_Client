#pragma once
#include "UI.h"

class UIList;
class UIButton;

class UIScrollView :
    public UI
{
private:
    UIList* m_pUIList = nullptr;
    UIButton* m_pUpBtn = nullptr, *m_pDownBtn = nullptr;

    int m_topIdx = 0, m_bottomIdx = 0;
    int m_maxItemViewCount = 0;

public:
    UIScrollView(const UIScrollView& _uiScrollView);
    UIScrollView(UI* _pParentPanel, UINT _width, UINT _height, INT _xpos, INT _ypos, UINT _itemWidth, UINT _itemHeight, float _itemVerticalInterval = 0.f);
    virtual ~UIScrollView();

    UIScrollView* Clone() const override { return new UIScrollView(*this); }

    UIList* GetUIList() const { return m_pUIList; }
    int GetTopIdx() const { return m_topIdx; }
    int GetBottomIdx() const { return m_bottomIdx; }
    int GetMaxItemViewCount() const { return m_maxItemViewCount; }

    void SetIdx(int _topIdx, int _bottomIdx);
    void AddItem(const std::wstring& _text);

    void Update() override;
    void Render() override;
    void Clear();
};

