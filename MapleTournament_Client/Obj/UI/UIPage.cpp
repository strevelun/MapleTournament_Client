#include "../../NetworkCore/NetworkManager.h"
#include "UIPage.h"
#include "UIButton.h"
#include "UIList.h"
#include "UIPanel.h"
#include "../../Managers/ResourceManager.h"
#include "../../Setting.h"
#include "../../GraphicCore/Graphics.h"

UIPage::UIPage(const UIPage& _uiPage)
	: UI(_uiPage)
{
	m_pUIList = _uiPage.m_pUIList->Clone();
	m_pPrevBtn = _uiPage.m_pPrevBtn->Clone();
	m_pNextBtn = _uiPage.m_pNextBtn->Clone();

	m_maxItemViewCount = _uiPage.m_maxItemViewCount;
	m_curPage = _uiPage.m_curPage;
	m_maxPageCount = _uiPage.m_maxPageCount;

}

UIPage::UIPage(UI* _pParentPanel, UINT _width, UINT _height, UINT _itemWidth, UINT _itemHeight, float _xpos, float _ypos, FLOAT _pivotX, FLOAT _pivotY, float _itemVerticalInterval) :
	UI(_pParentPanel, _width, _height, _xpos, _ypos, _pivotX, _pivotY)
{
	m_pUIList = new UIList(this, _width, _height, _itemWidth, _itemHeight, 0, 0, 0.f, 0.f);
	m_maxItemViewCount = _height / (_itemHeight + (u_int)_itemVerticalInterval);
	m_pUIList->SetItemVerticalInterval(_itemVerticalInterval);
	m_maxPageCount = 64 / 9; // TODO : 사용자 세팅 가능하도록
	if (64 % 9 != 0) m_maxPageCount++;

	Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_arrow_left_normal.png");
	m_pPrevBtn = new UIButton(_pParentPanel, 40, 40, 0.f, (float)_pParentPanel->GetHeight(), 0.f, 1.f);
	if (pBitmap) m_pPrevBtn->SetBitmap(pBitmap);
	pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_arrow_left_pressed.png");
	m_pPrevBtn->SetPressedBitmap(pBitmap);
	m_pPrevBtn->SetClickable(true);

	pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_arrow_right_normal.png");
	m_pNextBtn = new UIButton(_pParentPanel, 40, 40, (float)_pParentPanel->GetWidth(), (float)_pParentPanel->GetHeight(), 1.f, 1.f);
	if (pBitmap) m_pNextBtn->SetBitmap(pBitmap);
	pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_arrow_right_pressed.png");
	m_pNextBtn->SetPressedBitmap(pBitmap);
	m_pNextBtn->SetClickable(true);

	//Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Blue, &m_pBrush);
}

UIPage::~UIPage()
{
	delete m_pUIList;
	delete m_pPrevBtn;
	delete m_pNextBtn;

	//if (m_pBrush) m_pBrush->Release();
}

void UIPage::SetItemTemplate(UIPanel* _pItem)
{
	for (u_int i = 0; i < m_maxItemViewCount; i++)
	{
		m_pUIList->AddItem(_pItem->Clone());
	}
}

void UIPage::SetPrevBtnCallback(const std::function<void()>& _func)
{
	m_pPrevBtn->SetCallback(_func);
}

void UIPage::SetNextBtnCallback(const std::function<void()>& _func)
{
	m_pNextBtn->SetCallback(_func);
}

void UIPage::Update()
{
	m_pUIList->Update();
	m_pPrevBtn->Update();
	m_pNextBtn->Update();
}

void UIPage::Render()
{
	m_pUIList->Render();
	m_pPrevBtn->Render();
	m_pNextBtn->Render();

	//Graphics::GetInst()->GetRenderTarget()->DrawRectangle(m_rect, m_pBrush);
}

void UIPage::Reset()
{
	m_curPage = 0;
}
