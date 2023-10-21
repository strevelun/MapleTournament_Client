#pragma once

#include <WinSock2.h>
#include <dwrite.h>
#include <d2d1.h>
#include "Window.h"

class Window;
class TCPClient;

class GameApp
{
private:
	Window		m_window;
	TCPClient*	m_pClient = nullptr;

	IDWriteTextFormat* m_pTextFormat = nullptr;
	ID2D1SolidColorBrush* m_pBrush = nullptr;

public:
	GameApp(HINSTANCE _hInst, const wchar_t* _wndClassName);
	~GameApp();

	bool Init(int _nCmdShow, const wchar_t* _windowName, UINT _width, UINT _height);

	int Run();

private:
	void Input();
	void Update();
	void Render();
};

