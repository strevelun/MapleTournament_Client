#pragma once

#include <vector>
#include <map>
#include <type_traits>
#include <string>

#include "../Defines.h"

class UI;
class UIPanel;
class UIPopup;
class UIEditText;
class Graphics;

class UIManager
{
private:
	std::map<std::wstring, UI*>				m_mapUI;
	std::vector<UI*>						m_vecUI;
	std::vector<UIPanel*>			m_vecPopupUI; // UIPopup

public:
	bool AddUI(UI* _pUI);
	void AddPopupUI(UIPanel* _pUI);

	UI* FindUI(const std::wstring& _strName);
	bool RemoveUI(const std::wstring& _strName);

	//
	void PopPopupUI();
	//void DelPopupUI();

	bool Init();
	void Update();
	void Render();

	void Cleanup();

	SINGLETON(UIManager)
};

