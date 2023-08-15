#pragma once

#include <Windows.h>
#include <vector>
#include <functional>

class Mouse;

class InputManager
{
private:
	static InputManager* m_pInst;

	std::vector<WPARAM> m_vecKeyInput;

	HWND	m_hWnd;
	Mouse* m_pMouse;

	InputManager();
	~InputManager();

public:
	static InputManager* GetInst()
	{
		if (!m_pInst) m_pInst = new InputManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
		m_pInst = nullptr;
	}

	bool Init(HWND _hWnd);
	void Update();

	const std::vector<WPARAM>& GetVecKeyInput() const { return m_vecKeyInput; }
	void AddKeyInput(WPARAM _input);
	void ClearVecKeyInput() { m_vecKeyInput.clear(); }

	Mouse* GetMouse() const { return m_pMouse; }
};

