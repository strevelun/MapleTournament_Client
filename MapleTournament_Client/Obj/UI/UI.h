#pragma once

#include "../Obj.h"
#include <string>

class Mouse;
class UIText;

enum class eUIState
{
    Normal,
    On,
    Out,
    LButtonDown,
    Pressed,
    Click
};

class UI :
    public Obj
{
protected:
    bool m_bFocus = false;
    UI* m_pParentUI = nullptr;
    bool            m_bClickable = false;
    eUIState            m_eUIState = eUIState::Normal;

    POINT					m_tPosRelativeToParent;
    D2D1_RECT_F				m_rect;
    D2D1_SIZE_U				m_size;
    FLOAT					m_pivotX, m_pivotY;

public:
    UI();
    UI(UI* _pParentPanel, UINT _width, UINT _height, INT _xpos = 0, INT _ypos = 0, FLOAT _pivotX = 0.f, FLOAT _pivotY = 0.f);
    UI(const UI& _ui);
    virtual ~UI();

    virtual UI* Clone() const = 0;

    void SetParent(UI* _pUI) { m_pParentUI = _pUI; }
    void SetPos(INT _xpos, INT _ypos) override;
    void SetClickable(bool _bClickable);
    void SetSize(UINT _width, UINT _height); 
    void SetPivot(float _pivotX, float _pivotY); 

    UI* GetParent() const { return m_pParentUI; }
    UINT GetWidth() const { return m_size.width; }
    UINT GetHeight() const { return m_size.height; }
    FLOAT GetPivotX() const { return m_pivotX; }
    FLOAT GetPivotY() const { return m_pivotY; }
    UINT GetPosXRelativeToParent() const { return (UINT)m_tPosRelativeToParent.x; }
    UINT GetPosYRelativeToParent() const { return (UINT)m_tPosRelativeToParent.y; }

    bool IsClickable() const { return m_bClickable; }

    virtual void OnClick();
    virtual void OnLButtonDown();
    virtual void MousePressed();
    virtual void MouseOn();
    virtual void MouseOut();

    virtual void Update();
    virtual void Render() = 0;
};

