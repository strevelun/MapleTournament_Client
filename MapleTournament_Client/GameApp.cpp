#include "NetworkCore/TCPClient.h"
#include "GraphicCore/Graphics.h"
#include "GameApp.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"
#include "Scene/LoginScene.h"
#include "User.h"

#include <d2d1.h>

GameApp::GameApp(HINSTANCE _hInst, int _nCmdShow, const wchar_t* _wndClassName, const wchar_t* _windowName, UINT _width, UINT _height)
	: m_window(_hInst, _nCmdShow, _wndClassName, _windowName, _width, _height),
	m_pClient(nullptr),
	m_pGraphics(nullptr)
{
}

GameApp::~GameApp()
{
	if (m_pGraphics)
	{
		m_pGraphics->CleanupDevice();
		delete m_pGraphics;
	}

	if (m_pClient) delete m_pClient;

	ResourceManager::DestroyInst();
}

bool GameApp::Init()
{
	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) throw L"문제 발생 at WASStartup";

	m_pGraphics = new Graphics(m_window.GetHWnd());
	m_pClient = new TCPClient("192.168.219.167", 30001);

	if (!ResourceManager::GetInst()->Init(m_pGraphics->GetImagingFactory(), m_pGraphics->GetRenderTarget())) return false;

	SceneManager::GetInst()->ChangeScene(new LoginScene);

	return true;
}

int GameApp::Run()
{
	MSG msg = { 0 };

	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SceneManager::GetInst()->CheckSceneChange();
			Input();
			Update();
			Render();
		}
	}

	return (int)msg.wParam;
}

void GameApp::Input()
{
}

void GameApp::Update()
{

}

void GameApp::Render()
{
	ID2D1HwndRenderTarget* pRenderTarget = m_pGraphics->GetRenderTarget();
	pRenderTarget->BeginDraw();
	pRenderTarget->Clear();

	SceneManager::GetInst()->Render(pRenderTarget);

	pRenderTarget->EndDraw();
}
