#pragma once

#include <Windows.h>

class Window
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;

	const wchar_t* m_wndClassName;

public:
	Window();
	Window(HINSTANCE _hInst, int _nCmdShow, const wchar_t* _wndClassName, const wchar_t* _windowName, UINT _width, UINT _height);
	~Window();

	HWND	GetHWnd() const { return m_hWnd; }

	LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

