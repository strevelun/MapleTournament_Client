#include "Window.h"
#include <stdexcept>
#include "Managers/InputManager.h"

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

Window::Window(HINSTANCE _hInst, const wchar_t* _wndClassName)
	: m_hInst(_hInst), m_wndClassName(_wndClassName), m_hWnd(nullptr)
{

}

Window::~Window()
{
	UnregisterClass(m_wndClassName, m_hInst);
}

bool Window::Init(int _nCmdShow, const wchar_t* _windowName, UINT _width, UINT _height)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_wndClassName;
	wcex.hIconSm = LoadIcon(m_hInst, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, (LONG)_width, (LONG)_height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = CreateWindow(m_wndClassName, _windowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInst, this);
	if (!m_hWnd)
	{
		OutputDebugStringW(L"CreateWindow ½ÇÆÐ");
		return false;
	}

	ShowWindow(m_hWnd, _nCmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT Window::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_KEYDOWN:
		InputManager::GetInst()->AddKeyInput(wParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	Window* pWin = nullptr;

	if (_msg == WM_NCCREATE)
	{
		LPCREATESTRUCT pCS = (LPCREATESTRUCT)_lParam;
		SetLastError(0);
		pWin = (Window*)pCS->lpCreateParams;
		if (!SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)pWin))
		{
			if (GetLastError() != 0) return E_FAIL;
		}
	}
	else
	{
		pWin = reinterpret_cast<Window*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
	}

	if (pWin)
	{
		return pWin->Proc(_hWnd, _msg, _wParam, _lParam);
	}

	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}