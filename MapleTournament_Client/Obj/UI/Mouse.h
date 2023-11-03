#pragma once
#include "UI.h"

enum class eMouseState
{
    None,
    Down,
    Pressed,
    Click
};

class Mouse :
    public UI
{
private:
    HWND m_hWnd;
    POINT       m_tMousePos;

    eMouseState m_state = eMouseState::None;

    bool m_bPressed = false;

public:
    Mouse(HWND _hWnd);
    Mouse(const Mouse& _mouse);
    ~Mouse();

    Mouse* Clone() const override { return new Mouse(*this); }

    eMouseState GetState() const { return m_state; }
    int GetMouseXPos() const { return m_tMousePos.x; }
    int GetMouseYPos() const { return m_tMousePos.y; }


    void Update() override;
    void Render() override;
};

