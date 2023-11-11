#include "NetworkCore/TCPClient.h"
#include "NetworkCore/NetworkManager.h"
#include "GraphicCore/Graphics.h"
#include "GameApp.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"
#include "Managers/UIManager.h"
#include "Managers/InputManager.h"
#include "Managers/ObjectManager.h"
#include "InstructionQueue.h"
#include "Scene/LoginScene.h"
#include "Obj/UI/Mouse.h"
#include "Timer.h"
#include "Debug.h"

typedef unsigned short ushort;

#include <d2d1.h>

GameApp::GameApp(HINSTANCE _hInst, const wchar_t* _wndClassName)
	: m_window(_hInst, _wndClassName),
	m_pClient(nullptr)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

GameApp::~GameApp()
{
	if (m_pClient) delete m_pClient;

	if (m_pTextFormat) m_pTextFormat->Release();
	if (m_pBrush) m_pBrush->Release();

	//InstructionQueue::DestroyInst();
	InputManager::DestroyInst();
	SceneManager::DestroyInst();
	UIManager::DestroyInst();
	ObjectManager::DestroyInst();
	Timer::DestroyInst();
	Graphics::DestroyInst();
	ResourceManager::DestroyInst();
}

bool GameApp::Init(int _nCmdShow, const wchar_t* _windowName, UINT _width, UINT _height)
{
	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Debug::Log(L"WASStartup 에러");
		return false;
	}

	if (!m_window.Init(_nCmdShow, _windowName, _width, _height)) return false;
	if (!Graphics::GetInst()->Init(m_window.GetHWnd())) return false;
	
	Graphics::GetInst()->CreateTextFormat(L"Arial", 15.f, &m_pTextFormat);
	Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Black, &m_pBrush);

	m_pClient = new TCPClient();
	if (!m_pClient->Init("192.168.219.167", 30001)) return false;

	if (!ResourceManager::GetInst()->Init())
	{
		Debug::Log(L"NetworkManager 초기화 에러");
		return false;
	}
	if (!InputManager::GetInst()->Init(m_window.GetHWnd())) return false;
	if (!UIManager::GetInst()->Init()) return false;
	if (!ObjectManager::GetInst()->Init()) return false;
	if (!Timer::GetInst()->Init(FRAME_PER_SEC)) return false;
	//if (!InstructionQueue::GetInst()->Init()) return false;

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
				m_pClient->ReceivePacket();
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
	//InstructionQueue::GetInst()->Update();
	SceneManager::GetInst()->Update();
	UIManager::GetInst()->Update();
	InputManager::GetInst()->Update();
}

void GameApp::Render()
{
	Graphics::GetInst()->BeginDraw();

	SceneManager::GetInst()->Render();
	UIManager::GetInst()->Render();

	Mouse* pMouse = InputManager::GetInst()->GetMouse(); 
	wchar_t output[50];
	wsprintf(output, L"x: %d, y: %d", (int)pMouse->GetMouseXPos(), (int)pMouse->GetMouseYPos());
	D2D1_RECT_F outputRect = D2D1::RectF(5.0f, 5.0f, 500.0f, 20.0f);
	Graphics::GetInst()->GetRenderTarget()->DrawText(output, wcslen(output), m_pTextFormat, outputRect, m_pBrush);

	Graphics::GetInst()->EndDraw();
}
