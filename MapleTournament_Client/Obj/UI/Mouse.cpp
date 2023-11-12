#include "Mouse.h"
#include "../../Bitmap.h"
#include "../../GraphicCore/Graphics.h"

Mouse::Mouse(HWND _hWnd) :
	m_hWnd(_hWnd)
{
}

Mouse::Mouse(const Mouse& _mouse) 
	: UI(_mouse)
{
	m_hWnd = _mouse.m_hWnd;
	m_state = _mouse.m_state;
	m_bPressed = _mouse.m_bPressed;
}

Mouse::~Mouse()
{
}

void Mouse::Update()
{
	GetCursorPos(&m_tMousePos);
	ScreenToClient(m_hWnd, &m_tMousePos);

	bool bLButton = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

	if (m_state == eMouseState::Click)
	{
		m_state = eMouseState::None;
	}
	else if (bLButton && m_state == eMouseState::None)
	{
		m_state = eMouseState::Down;
	}
	else if (bLButton && m_state == eMouseState::Down)
	{
		m_state = eMouseState::Pressed;
	}
	else if (!bLButton && m_state == eMouseState::Pressed)
	{
		m_state = eMouseState::Click;
	}
}

void Mouse::Render()
{
	if (m_pHoverBitmap)
	{
		m_rectHoverBitmap = { (float)m_tMousePos.x, (float)m_tMousePos.y - 150, (float)m_tMousePos.x + 150, (float)m_tMousePos.y };

		Graphics::GetInst()->GetRenderTarget()->DrawBitmap(m_pHoverBitmap->GetBitmap(), m_rectHoverBitmap);
	}
}
