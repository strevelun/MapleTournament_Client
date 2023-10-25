#include "UIScrollView.h"
#include "UIList.h"
#include "UIButton.h"
#include "UIPanel.h"
#include "UIText.h"
#include "../../GraphicCore/Graphics.h"
#include "../../Managers/ResourceManager.h"

UIScrollView::UIScrollView(const UIScrollView& _uiScrollView)
	: UI(_uiScrollView)
{
	m_topIdx = _uiScrollView.m_topIdx;
	m_bottomIdx = _uiScrollView.m_bottomIdx;
	m_maxItemViewCount = _uiScrollView.m_maxItemViewCount;

	m_pUIList = _uiScrollView.m_pUIList->Clone();
	m_pUpBtn = _uiScrollView.m_pUpBtn->Clone();
	m_pDownBtn = _uiScrollView.m_pDownBtn->Clone();
}

UIScrollView::UIScrollView(UI* _pParentPanel, UINT _width, UINT _height, INT _xpos, INT _ypos, UINT _itemWidth, UINT _itemHeight, float _itemVerticalInterval) :
	UI(_pParentPanel, _width, _height, _xpos, _ypos)
{
	m_pUIList = new UIList(this, _width, _height, _itemWidth, _itemHeight, 0, 0, 0.f, 0.f);
	m_maxItemViewCount = _height / (_itemHeight + _itemVerticalInterval);
	m_pUIList->SetItemVerticalInterval(_itemVerticalInterval);

	Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_arrow_up.png");
	m_pUpBtn = new UIButton(this, 50, 50, _width, 0, 1.f);
	if (pBitmap) m_pUpBtn->SetBitmap(pBitmap);
	m_pUpBtn->SetClickable(true);
	m_pUpBtn->SetCallback([this]() 
		{ 
			if (m_topIdx <= 0) return;

			m_topIdx--; m_bottomIdx--; 
		});

	pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_arrow_down.png");
	m_pDownBtn = new UIButton(this, 50, 50, _width, _height, 1.f, 1.f);
	if (pBitmap) m_pDownBtn->SetBitmap(pBitmap);

	m_pDownBtn->SetClickable(true);
	m_pDownBtn->SetCallback([this]() 
		{
			if (m_pUIList->GetItemCount() < m_maxItemViewCount) return;
			if (m_bottomIdx >= m_pUIList->GetItemCount()) return;

			m_topIdx++; m_bottomIdx++; 
		});
}

UIScrollView::~UIScrollView()
{
	delete m_pUIList;
	delete m_pUpBtn;
	delete m_pDownBtn;
}

void UIScrollView::SetIdx(int _topIdx, int _bottomIdx)
{
	m_topIdx = _topIdx;
	m_bottomIdx = _bottomIdx;
}

void UIScrollView::AddItem(const std::wstring& _text, float _textSize)
{
	size_t ct = m_pUIList->GetItemCount();
	if (m_maxItemViewCount <= ct)
		SetIdx(ct - m_maxItemViewCount + 1, ct + 1);

	m_pUIList->AddItem(_text, _textSize);
}

void UIScrollView::Update()
{
	size_t itemCount = m_pUIList->GetItemCount();
	if (itemCount > m_bottomIdx - m_topIdx)
	{
		if(itemCount <= m_maxItemViewCount)
			m_bottomIdx++;
	}
	else if (itemCount < m_bottomIdx - m_topIdx)
		m_bottomIdx--;

	m_pUIList->Update();
	m_pUpBtn->Update();
	m_pDownBtn->Update();
}

void UIScrollView::Render()
{
	m_pUIList->Render(m_topIdx, m_bottomIdx);

	m_pUpBtn->Render();
	m_pDownBtn->Render();
}

void UIScrollView::Clear()
{
	m_topIdx = 0, m_bottomIdx = 0;

	m_pUIList->RemoveAllItems();
}
