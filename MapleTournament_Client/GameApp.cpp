#include "NetworkCore/TCPClient.h"
#include "NetworkCore/NetworkManager.h"
#include "GraphicCore/Graphics.h"
#include "GameApp.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"
#include "Managers/InputManager.h"
#include "Scene/LoginScene.h"
#include "User.h"
#include "Obj/UI/Mouse.h"

#include <d2d1.h>

GameApp::GameApp(HINSTANCE _hInst, const wchar_t* _wndClassName)
	: m_window(_hInst, _wndClassName),
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

	ResourceManager::DestroyInst(); // TODO : m_mapBitmap 해제
	InputManager::DestroyInst();
	SceneManager::DestroyInst();
}

bool GameApp::Init(int _nCmdShow, const wchar_t* _windowName, UINT _width, UINT _height)
{
	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		OutputDebugStringW(L"WASStartup 에러");
		return false;
	}

	if (!m_window.Init(_nCmdShow, _windowName, _width, _height)) return false;

	m_pGraphics = new Graphics(m_window.GetHWnd());
	if (!m_pGraphics->Init()) return false;

	m_pClient = new TCPClient();
	if (!m_pClient->Init()) return false;

	if (!ResourceManager::GetInst()->Init(m_pGraphics->GetImagingFactory(), m_pGraphics->GetRenderTarget()))
	{
		OutputDebugStringW(L"NetworkManager 초기화 에러");
		return false;
	}
	if (!InputManager::GetInst()->Init(m_window.GetHWnd())) return false;

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
	InputManager::GetInst()->Update();
	SceneManager::GetInst()->Update();
}

void GameApp::Render()
{
	m_pGraphics->BeginDraw();

	m_pGraphics->Render();

	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	m_pGraphics->DrawMouseCoordinates(pMouse->GetPosX(), pMouse->GetPosY());

	m_pGraphics->EndDraw();
}
