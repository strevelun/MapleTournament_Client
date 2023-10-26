#include "UI.h"
#include "../../Bitmap.h"
#include "../../GraphicCore/Graphics.h"
#include "Mouse.h"
#include "../../Managers/InputManager.h"
#include "UIText.h"

#include <d2d1.h>

UI::UI()
{
}

UI::UI(UI* _pParentPanel, UINT _width, UINT _height, INT _xpos, INT _ypos, FLOAT _pivotX, FLOAT _pivotY) :
	Obj(_xpos, _ypos), m_size{_width, _height}, m_pivotX(_pivotX), m_pivotY(_pivotY),
	m_pParentUI(_pParentPanel)
{
	m_tPosRelativeToParent.x = _xpos - (m_pivotX * m_size.width);
	m_tPosRelativeToParent.y = _ypos - (m_pivotY * m_size.height);

	SetPos(_xpos, _ypos);
}

UI::UI(const UI& _ui) :
	Obj(_ui)
{
	m_tPosRelativeToParent = _ui.m_tPosRelativeToParent;
	m_bFocus = _ui.m_bFocus;
	m_pParentUI = _ui.m_pParentUI; 
	m_bClickable = _ui.m_bClickable;
	m_eUIState = _ui.m_eUIState;
	m_rect = _ui.m_rect;
	m_size = _ui.m_size;
	m_pivotX = _ui.m_pivotX;
	m_pivotY = _ui.m_pivotY;
}

UI::~UI()
{
}

void UI::SetPos(INT _xpos, INT _ypos) 
{
	if (m_pParentUI)
	{
		_xpos += m_pParentUI->GetPosX();
		_ypos += m_pParentUI->GetPosY();
	}
	m_tPos.x = _xpos - (m_pivotX * m_size.width);
	m_tPos.y = _ypos - (m_pivotY * m_size.height);
	m_rect = { (float)m_tPos.x, (float)m_tPos.y, (float)m_tPos.x + m_size.width, (float)m_tPos.y + m_size.height };
}

void UI::SetClickable(bool _bClickable)
{
	m_bClickable = _bClickable;
}

void UI::SetSize(UINT _width, UINT _height)
{
	m_size.width = _width;
	m_size.height = _height;
	m_rect = { m_tPos.x - (m_pivotX * m_size.width), m_tPos.y - (m_pivotY * m_size.height),
	   m_tPos.x + m_size.width - (m_pivotX * m_size.width), m_tPos.y + m_size.height - (m_pivotY * m_size.height) };
}

void UI::SetPivot(float _pivotX, float _pivotY)
{
	m_pivotX = _pivotX;
	m_pivotY = _pivotY;
	m_rect = { m_tPos.x - (m_pivotX * m_size.width), m_tPos.y - (m_pivotY * m_size.height),
	   m_tPos.x + m_size.width - (m_pivotX * m_size.width), m_tPos.y + m_size.height - (m_pivotY * m_size.height) };

}

void UI::OnClick()
{
	if (m_eUIState == eUIState::LButtonDown || m_eUIState == eUIState::Pressed)
	{
		m_eUIState = eUIState::Click;
	}
}

void UI::OnLButtonDown()
{
	if (m_eUIState == eUIState::On)
	{
		m_eUIState = eUIState::LButtonDown;
	}
}

void UI::MousePressed()
{
	if (m_eUIState == eUIState::LButtonDown)
	{
		m_eUIState = eUIState::Pressed;
	}
}

void UI::MouseOn()
{
	if (m_eUIState == eUIState::Click)
	{
		m_eUIState = eUIState::Normal;
	}
	else if (m_eUIState == eUIState::Normal)
	{
		m_eUIState = eUIState::On;
	}
}

void UI::MouseOut()
{
	if (m_eUIState == eUIState::Normal) return;

	if (m_eUIState != eUIState::Out)			m_eUIState = eUIState::Out;
	else if(m_eUIState == eUIState::Out)		m_eUIState = eUIState::Normal;
}

void UI::Update()
{
	if (m_bClickable)
	{
		Mouse* pMouse = InputManager::GetInst()->GetMouse();
		int mouseXPos = pMouse->GetPosX();
		int mouseYPos = pMouse->GetPosY();
		eMouseState state = pMouse->GetState();

		if (m_rect.left <= mouseXPos && mouseXPos <= m_rect.right && m_rect.top <= mouseYPos && mouseYPos <= m_rect.bottom)
		{
			UI::MouseOn();

			if (state == eMouseState::Click)
			{
				m_bFocus = true;
				UI::OnClick();
			}
			else if (state == eMouseState::Down)
			{
				UI::OnLButtonDown();
			}
			else if (state == eMouseState::Pressed)
			{
				UI::MousePressed();
			}
		}
		else
		{
			UI::MouseOut();
			if (state == eMouseState::Click)
				m_bFocus = false;
		}
	}
}