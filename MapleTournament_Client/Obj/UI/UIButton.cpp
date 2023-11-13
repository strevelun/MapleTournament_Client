#include "UIButton.h"
#include "Mouse.h"
#include "../../Managers/InputManager.h"
#include "UIText.h"
#include "../../GraphicCore/Graphics.h"
#include "../../Bitmap.h"
#include "../../Debug.h"

UIButton::UIButton(const UIButton& _uiButton)
	: UI(_uiButton)
{
	if(_uiButton.m_pUIText)
		m_pUIText = _uiButton.m_pUIText->Clone();
	m_pBitmap = _uiButton.m_pBitmap;
	m_Callback = _uiButton.m_Callback;
}

UIButton::UIButton(UI* _pParentPanel, UINT _width, UINT _height, float _xpos, float _ypos,  FLOAT _pivotX, FLOAT _pivotY) :
	UI(_pParentPanel, _width, _height, _xpos, _ypos,  _pivotX, _pivotY)
{
}

UIButton::~UIButton()
{
	if (m_pUIText) delete m_pUIText;
}

void UIButton::Update()
{
	UI::Update();

	switch (m_eUIState)
	{
	case eUIState::Normal:
		break;
	case eUIState::On:
		MouseOn();
		break;
	case eUIState::Out:
		MouseOut();
		break;
	case eUIState::Hover:
		MouseHover();
		break;
	case eUIState::LButtonDown:
		OnLButtonDown();
		break;
	case eUIState::Pressed:
		MousePressed();
		break;
	case eUIState::Click:
		OnClick();
		m_eUIState = eUIState::Normal;
		break;
	}
}

void UIButton::Render()
{
	if (m_pBitmap)		Graphics::GetInst()->GetRenderTarget()->DrawBitmap(m_pBitmap->GetBitmap(), m_rect);
	if (m_pUIText) m_pUIText->Render();
}

void UIButton::SetBitmap(Bitmap* _pBitmap)
{
	m_pBitmap = _pBitmap;
	m_pNormalBitmap = _pBitmap;
}

void UIButton::SetUIText(UIText* _pUIText)
{
	if (m_pUIText) delete m_pUIText;
	m_pUIText = _pUIText;
}

UIText* UIButton::GetUIText()
{
	return m_pUIText;
}

void UIButton::OnClick()
{
	if (m_Callback) m_Callback();
	
	if (m_pMouseHoverBitmap)	m_pMouse->SetHoverBitmap(nullptr);

	if (m_pPressedBitmap)	m_pBitmap = m_pNormalBitmap;
}

void UIButton::OnLButtonDown()
{
	if (m_pPressedBitmap)	m_pBitmap = m_pPressedBitmap;
}

void UIButton::MousePressed()
{
}

void UIButton::MouseOn()
{
	if(m_pMouseHoverBitmap)	m_pMouse->SetHoverBitmap(m_pMouseHoverBitmap);

}

void UIButton::MouseOut()
{
	if(m_pMouseHoverBitmap && m_pMouseHoverBitmap == m_pMouse->GetHoverBitmap())
		m_pMouse->SetHoverBitmap(nullptr);

	if (m_pPressedBitmap)	m_pBitmap = m_pNormalBitmap;
}

void UIButton::MouseHover()
{
	
}

