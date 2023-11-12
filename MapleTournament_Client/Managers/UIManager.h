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
	std::map<std::wstring, UIPanel*>		m_mapPopupUI;
	std::vector<UIPanel*>			m_vecPopupUI; 

public:
	void AddUI(UI* _pUI);
	void SetPopupUI(UIPanel* _pUI);
	void AddPopupUI(UIPanel* _pUI);

	UI* FindUI(const std::wstring& _strName);
	bool RemoveUI(const std::wstring& _strName);

	void PopPopupUI();

	bool PopupUIExist();

	bool Init();
	void Update();
	void Render();

	void Cleanup();

	SINGLETON(UIManager)
};

