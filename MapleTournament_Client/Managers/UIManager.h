#pragma once

#include <vector>
#include <type_traits>
#include <string>

class UI;
class UIPanel;
class UIPopup;
class UIEditText;
class Graphics;

class UIManager
{
private:
	static UIManager* m_pInst;

	std::vector<UI*>				m_vecUI;
	std::vector<UIPanel*>			m_vecPopupUI; // UIPopup

	UIManager();
	~UIManager();

public:
	static UIManager* GetInst()
	{
		if (!m_pInst) m_pInst = new UIManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
		m_pInst = nullptr;
	}

	void AddUI(UI* _pUI);
	void AddPopupUI(UIPanel* _pUI);

	UI* FindUI(const std::wstring& _strName);

	//
	void PopPopupUI();
	//void DelPopupUI();

	bool Init();
	void Update();
	void Render(Graphics* _pGraphics);

	void Cleanup();
};

