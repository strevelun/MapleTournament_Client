#pragma once

#include <Windows.h>
#include <vector>
#include <functional>

#include "../Defines.h"

class Mouse;

class InputManager
{
private:
	std::vector<WPARAM> m_vecKeyInput;
	std::vector<int>		m_vkInput;

	HWND	m_hWnd;
	Mouse* m_pMouse;

public:
	bool Init(HWND _hWnd);
	void Update();

	const std::vector<WPARAM>& GetVecKeyInput() const { return m_vecKeyInput; }
	void RegisterVKKey(int _vkKey);
	void AddChar(WPARAM _input);
	void ClearVecKeyInput() { m_vecKeyInput.clear(); }

	Mouse* GetMouse() const { return m_pMouse; }

	SINGLETON(InputManager)
};

