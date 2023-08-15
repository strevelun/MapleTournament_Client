#pragma once

#include <Windows.h>

class Window
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;

	const wchar_t* m_wndClassName;

public:
	Window(HINSTANCE _hInst, const wchar_t* _wndClassName);
	~Window();

	HWND	GetHWnd() const { return m_hWnd; }

	bool Init(int _nCmdShow, const wchar_t* _windowName, UINT _width, UINT _height);

	LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

