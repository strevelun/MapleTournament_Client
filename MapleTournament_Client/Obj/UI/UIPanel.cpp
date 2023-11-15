#include "UIPanel.h"
#include "../../Bitmap.h"
#include "../../GraphicCore/Graphics.h"

#include <d2d1.h>

UIPanel::UIPanel(const UIPanel& _ui)
	: UI(_ui)
{
	for (auto ui : _ui.m_vecMemberUI) 
	{
		UI* pUI = ui->Clone();
		pUI->SetParent(this);
		m_vecMemberUI.push_back(pUI);
	}

	m_pBitmap = _ui.m_pBitmap;

	//Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Red, &m_pBrush);
	
}

UIPanel::UIPanel(UI* _pParentPanel, UINT _width, UINT _height, float _xpos, float _ypos, FLOAT _pivotX, FLOAT _pivotY) :
	UI(_pParentPanel, _width, _height, _xpos, _ypos, _pivotX, _pivotY)
{
	//Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Red, &m_pBrush);
}

UIPanel::~UIPanel()
{
	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();
	for (; iter != iterEnd; ++iter)
		delete* iter;

	//if (m_pBrush) m_pBrush->Release();
}

void UIPanel::AddChildUI(UI* _pUI)
{
	if (!_pUI) return; 

	m_vecMemberUI.push_back(_pUI);
}

UI* UIPanel::FindChildUI(const std::wstring& _strName)
{
	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == _strName)
			return *iter;
	}
	return nullptr;
}

void UIPanel::SetActive(bool _bActive)
{
	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetActive(_bActive);
	}

	m_bActive = _bActive;
}

void UIPanel::SetPos(float _xpos, float _ypos)
{
	UI::SetPos(_xpos, _ypos);

	size_t size = m_vecMemberUI.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecMemberUI[i]->SetPos(m_vecMemberUI[i]->GetPosXRelativeToParent(), m_vecMemberUI[i]->GetPosYRelativeToParent());
	}
}

void UIPanel::SetClickable(bool _bClickable)
{
	UI::SetClickable(_bClickable);

	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetClickable(_bClickable);
	}
}

void UIPanel::SetPopup(bool _isPopup)
{
	m_isPopup = _isPopup;

	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetPopup(_isPopup);
	}
}

void UIPanel::Update()
{
	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsAlive()) // Scene 바뀔때 alive = false
		{
			delete* iter;
			iter = m_vecMemberUI.erase(iter);
		}
		else
		{
			if ((*iter)->IsActive())
				(*iter)->Update();
			++iter;
		}
	}
}

void UIPanel::Render()
{
	// 자기자신 비트맵 출력 후 벡터에 있는 모든 UI Render하기
	if (m_pBitmap)		Graphics::GetInst()->GetRenderTarget()->DrawBitmap(m_pBitmap->GetBitmap(), m_rect);

	size_t size = m_vecMemberUI.size();
	for (size_t i = 0; i< size; i++)
	{
		if (!m_vecMemberUI[i]->IsActive()) continue;
		m_vecMemberUI[i]->Render();
	}

	//Graphics::GetInst()->GetRenderTarget()->DrawRectangle(m_rect, m_pBrush);
}
