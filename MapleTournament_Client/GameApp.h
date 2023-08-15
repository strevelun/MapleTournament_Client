#pragma once

#include <WinSock2.h>
#include "Window.h"

class Window;
class Graphics;
class TCPClient;

class GameApp
{
private:
	Graphics*	m_pGraphics;
	Window		m_window;
	TCPClient*	m_pClient;

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

