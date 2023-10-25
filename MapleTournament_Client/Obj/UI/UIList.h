#pragma once
#include "UI.h"

#include <vector>

class UIText;
class UIPanel;

class UIList :
    public UI
{
private:
    std::vector<UI*> m_vecItem;
    
    UINT    m_itemWidth = 0, m_itemHeight = 0;
    float m_itemVerticalInterval = 0.0f;

public:
    UIList(const UIList& _ui);
    UIList(UI* _pParentPanel, UINT _width, UINT _height, UINT _itemWidth, UINT _itemHeight, INT _xpos = 0, INT _ypos = 0, FLOAT _pivotX = 0.f, FLOAT _pivotY = 0.f);
    ~UIList();

    UIList* Clone() const override { return new UIList(*this); }

    void AddItem(std::wstring _text, float _textSize);
    void AddItem(UIPanel* _pItem);
    bool RemoveItem(std::wstring _strName);
    UI* FindItem(std::wstring _strName);
    UI* GetIdxItem(u_int _idx);
    void RemoveAllItems(); // ¾À ¹Ù²ð¶§

    size_t GetItemCount() const { return m_vecItem.size(); }
    UINT GetItemHeight() const { return m_itemHeight; }

    void SetItemVerticalInterval(float _interval) { m_itemVerticalInterval = _interval; }

    void Update() override;
    void Render() override;
    void Render(int _from, int _to);
};

