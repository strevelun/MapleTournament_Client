#pragma once

#include <WinSock2.h>
#include "App.h"
#include "Window.h"

class Window;
class Graphics;
class TCPClient;

class GameApp : public App
{
private:
	Graphics*	m_pGraphics;
	Window		m_window;
	TCPClient*	m_pClient;

	//USHORT		m_id;

public:
	GameApp(HINSTANCE _hInst, int _nCmdShow, const wchar_t* _wndClassName, const wchar_t* _windowName, UINT _width, UINT _height);
	~GameApp();

	bool Init();

	int Run() override;

private:
	void Input();
	void Update() override;
	void Render();
};

