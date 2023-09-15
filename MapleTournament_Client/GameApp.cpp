#include "NetworkCore/TCPClient.h"
#include "NetworkCore/NetworkManager.h"
#include "GraphicCore/Graphics.h"
#include "GameApp.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"
#include "Managers/UIManager.h"
#include "Managers/InputManager.h"
#include "Managers/ObjectManager.h"
#include "Scene/LoginScene.h"
#include "Obj/UI/Mouse.h"
#include "Timer.h"

typedef unsigned short ushort;

#include <d2d1.h>

GameApp::GameApp(HINSTANCE _hInst, const wchar_t* _wndClassName)
	: m_window(_hInst, _wndClassName),
	m_pClient(nullptr),
	m_pGraphics(nullptr)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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
	UIManager::DestroyInst();
	ObjectManager::DestroyInst();
	Timer::DestroyInst();
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
	if (!UIManager::GetInst()->Init()) return false;
	if (!ObjectManager::GetInst()->Init()) return false;
	if (!Timer::GetInst()->Init(120)) return false;

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
			if (msg.message == WM_QUIT)
			{
				char buffer[255];
				ushort count = sizeof(ushort);
				*(ushort*)(buffer + count) = (ushort)ePacketType::C_Exit;					count += sizeof(ushort);
				*(ushort*)buffer = count;
				NetworkManager::GetInst()->Send(buffer);
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (Timer::GetInst()->Update())
			{
				SceneManager::GetInst()->CheckSceneChange();
				Input();
				Update();
				Render();
			}
		}
	}

	return (int)msg.wParam;
}

void GameApp::Input()
{
}

void GameApp::Update()
{
	SceneManager::GetInst()->Update();
	UIManager::GetInst()->Update();
	InputManager::GetInst()->Update();
}

void GameApp::Render()
{
	m_pGraphics->BeginDraw();

	SceneManager::GetInst()->Render(m_pGraphics);
	UIManager::GetInst()->Render(m_pGraphics);

	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	m_pGraphics->DrawMouseCoordinates(pMouse->GetPosX(), pMouse->GetPosY());

	m_pGraphics->EndDraw();
}
