#include "UIPanel.h"
#include "../../Bitmap.h"
#include "../../GraphicCore/Graphics.h"

#include <d2d1.h>

UIPanel::UIPanel(const UIPanel& _ui)
	: UI(_ui)
{
	for (auto ui : _ui.m_vecMemberUI) {
		m_vecMemberUI.push_back(ui->Clone());
	}

	m_pBitmap = _ui.m_pBitmap;

	Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Red, &m_pBrush);
	
}

UIPanel::UIPanel(UI* _pParentPanel, UINT _width, UINT _height, INT _xpos, INT _ypos, FLOAT _pivotX, FLOAT _pivotY) :
	UI(_pParentPanel, _width, _height, _xpos, _ypos, _pivotX, _pivotY)
{
	Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Red, &m_pBrush);
}

UIPanel::~UIPanel()
{
	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();
	for (; iter != iterEnd; iter++)
		delete* iter;

	if (m_pBrush) m_pBrush->Release();
}

void UIPanel::AddChildUI(UI* _pUI)
{
	if (!_pUI) return; 
	//int posY = _pUI->GetPosY();
	//_pUI->SetPos(posX + m_tPos.x - (m_size.width * m_pivotX), posY + m_tPos.y - (m_size.height * m_pivotY));
	//_pUI->SetPos(posX, posY);
	m_vecMemberUI.push_back(_pUI);
}

UI* UIPanel::FindChildUI(const std::wstring& _strName)
{
	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetName() == _strName)
			return *iter;
	}
	return nullptr;
}

// Panel안에 Panle이 있는 경우?
void UIPanel::SetActive(bool _bActive)
{
	std::vector<UI*>::iterator iter = m_vecMemberUI.begin();
	std::vector<UI*>::iterator iterEnd = m_vecMemberUI.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->SetActive(_bActive);
	}

	m_bActive = _bActive;
}

void UIPanel::SetPos(INT _xpos, INT _ypos)
{
	UI::SetPos(_xpos, _ypos);
	size_t size = m_vecMemberUI.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecMemberUI[i]->SetPos(m_vecMemberUI[i]->GetPosX() - m_vecMemberUI[i]->GetParent()->GetPosX(), _ypos+ m_vecMemberUI[i]->GetPosY() - m_vecMemberUI[i]->GetParent()->GetPosY());
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
			(*iter)->Update();
			iter++;
		}
	}
	/*
	size_t size = m_vecMemberUI.size() ;
	for (int i = 0; i < size; i++)
	{
		m_vecMemberUI[i]->Update();
	}
	*/
}

void UIPanel::Render()
{
	// 자기자신 비트맵 출력 후 벡터에 있는 모든 UI Render하기
	if (m_pBitmap)		Graphics::GetInst()->GetRenderTarget()->DrawBitmap(m_pBitmap->GetBitmap(), m_rect);

	unsigned int size = m_vecMemberUI.size();
	for (int i = 0; i< size; i++)
	{
		m_vecMemberUI[i]->Render();
	}

	Graphics::GetInst()->GetRenderTarget()->DrawRectangle(m_rect, m_pBrush);
}
