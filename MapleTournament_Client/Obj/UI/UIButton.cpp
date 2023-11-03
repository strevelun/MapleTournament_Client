#include "UIButton.h"
#include "Mouse.h"
#include "../../Managers/InputManager.h"
#include "UIText.h"
#include "../../GraphicCore/Graphics.h"
#include "../../Bitmap.h"

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
	//SetPos(m_tPos.x, m_tPos.y - 3);
}

void UIButton::OnLButtonDown()
{
	//SetPos(m_tPos.x, m_tPos.y + 3);
}

void UIButton::MousePressed()
{
}

void UIButton::MouseOn()
{

}

void UIButton::MouseOut()
{
	//SetPos(m_tPos.x, m_tPos.y - 3);
}

