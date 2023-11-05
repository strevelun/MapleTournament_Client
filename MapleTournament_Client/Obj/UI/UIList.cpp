#include "UIList.h"
#include "UIPanel.h"
#include "../../Constants.h"
#include "../../Setting.h"
#include "UIText.h"

#include <vector>

typedef unsigned short ushort;

UIList::UIList(const UIList& _ui)
	: UI(_ui)
{
	m_itemWidth = _ui.m_itemWidth;
	m_itemHeight = _ui.m_itemHeight;
	m_itemVerticalInterval = _ui.m_itemVerticalInterval;

	for (auto ui : _ui.m_vecItem) {
		m_vecItem.push_back(ui->Clone());
	}
}

UIList::UIList(UI* _pParentPanel, UINT _width, UINT _height, UINT _itemWidth, UINT _itemHeight, float _xpos, float _ypos, FLOAT _pivotX, FLOAT _pivotY) :
	UI(_pParentPanel, _width, _height, _xpos, _ypos, _pivotX, _pivotY), m_itemWidth(_itemWidth),m_itemHeight(_itemHeight)
{
}

UIList::~UIList()
{
	std::vector<UI*>::iterator iter = m_vecItem.begin();
	std::vector<UI*>::iterator iterEnd = m_vecItem.end();
	for (; iter != iterEnd; ++iter)
	{
		delete* iter;
	}
}

void UIList::AddItem(std::wstring _text, float _textSize)
{
	size_t itemSize = m_vecItem.size();
	UIPanel* pPanel = new UIPanel(this, m_itemWidth, m_itemHeight);
	UIText * pText = new UIText(pPanel, _text, _textSize);
	pPanel->AddChildUI(pText);
	pPanel->SetPos(0, pPanel->GetHeight() * itemSize + (itemSize > 0 ? m_itemVerticalInterval * itemSize : 0));
	m_vecItem.push_back(pPanel);
}

void UIList::AddItem(UIPanel* _pItem)
{
	size_t itemSize = m_vecItem.size();
	_pItem->SetPos(0, _pItem->GetHeight() * itemSize + (itemSize > 0 ? m_itemVerticalInterval * itemSize : 0));
	m_vecItem.push_back(_pItem);
}

bool UIList::RemoveItem(std::wstring _strName) // 중간에 아이템이 빠지면 뒤에꺼 전부 앞으로 밀기
{
	std::vector<UI*>::iterator iter = m_vecItem.begin();
	std::vector<UI*>::iterator iterEnd = m_vecItem.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == _strName)
		{
			delete* iter;
			m_vecItem.erase(iter);
			return true;
		}
	}
	return false;
}

UI* UIList::FindItem(std::wstring _strName)
{
	std::vector<UI*>::iterator iter = m_vecItem.begin();
	std::vector<UI*>::iterator iterEnd = m_vecItem.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == _strName)
			return *iter;
	}
	return nullptr;
}

UI* UIList::GetIdxItem(u_int _idx)
{
	if (_idx >= m_vecItem.size()) return nullptr;

	return m_vecItem[_idx];
}

void UIList::RemoveAllItems()
{
	std::vector<UI*>::iterator iter = m_vecItem.begin();
	std::vector<UI*>::iterator iterEnd = m_vecItem.end();


	for (; iter != iterEnd; ++iter)
	{
		delete (*iter);
		//iter = m_vecItem.erase(iter);
	}
	m_vecItem.clear();
}

UINT UIList::GetCurViewItemLineCount() const
{
	int h = 0, line = 0;

	for (auto& item : m_vecItem)
	{
		if (h >= m_size.height) break;
		line += item->GetHeight() / m_itemHeight;
		h += item->GetHeight();
	}

	return line;
}

void UIList::Update()
{
	for (auto& item : m_vecItem)
	{
		if(item->IsActive())
			item->Update();
	}
}

void UIList::Render() 
{
	for (auto& item : m_vecItem)
	{
		if (item->IsActive())
			item->Render();
	}
}

void UIList::Render(int _from, int _to)
{
	
	if (_from < 0 || _to > m_vecItem.size()) return;

	//int h = 0;

	for (int i = _from, j=0; i < _to; i++, j++)
	{
		//if (h > m_size.height) break;
		if (!m_vecItem[i]->IsActive()) continue;

		m_vecItem[i]->SetPos(0, m_vecItem[i]->GetHeight() * j);

		//m_vecItem[i]->SetPos(0, h);
		//h += m_vecItem[i]->GetHeight();

		m_vecItem[i]->Render();
	}
	
}
