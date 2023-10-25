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

    eMouseState m_state = eMouseState::None;

    bool m_bPressed = false;

public:
    Mouse(HWND _hWnd);
    Mouse(const Mouse& _mouse);
    ~Mouse();

    Mouse* Clone() const override { return new Mouse(*this); }

    eMouseState GetState() const { return m_state; }


    void Update() override;
    void Render() override;
};

