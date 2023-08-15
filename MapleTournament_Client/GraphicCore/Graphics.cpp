#include "Graphics.h"
#include "../Window.h"
#include "../Managers/SceneManager.h"
#include "../Managers/InputManager.h"
#include "../Bitmap.h"

#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwrite.lib")

Graphics::Graphics(HWND _hWnd) :
	m_hWnd(_hWnd)
{

}

Graphics::~Graphics()
{
}

bool Graphics::Init()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	if (FAILED(hr))
	{
		OutputDebugStringW((L"D2D1CreateFactory returned : " + std::to_wstring(hr)).c_str());
		return false;
	}

	GetClientRect(m_hWnd, &m_clientRect);
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE),
		D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(m_clientRect.right - m_clientRect.left, m_clientRect.bottom - m_clientRect.top)),
		&m_pRenderTarget);
	if (FAILED(hr))
	{
		OutputDebugStringW((L"CreateHwndRenderTarget returned : " + std::to_wstring(hr)).c_str());
		return false;
	}

	hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		OutputDebugStringW((L"CoInitialize returned : " + std::to_wstring(hr)).c_str());
		return false;
	}

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	if (FAILED(hr))
	{
		OutputDebugStringW((L"CoCreateInstance returned  : " + std::to_wstring(hr)).c_str());
		return false;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (FAILED(hr))
	{
		OutputDebugStringW((L"DWriteCreateFactory returned  : " + std::to_wstring(hr)).c_str());
		return false;
	}

	hr = m_pDWriteFactory->CreateTextFormat(L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		25.0f, L"en-US", &m_pTextFormat);
	if (FAILED(hr))
	{
		OutputDebugStringW((L"CreateTextFormat returned : " + std::to_wstring(hr)).c_str());
		return false;
	}

	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBrush);
	if (FAILED(hr))
	{
		OutputDebugStringW((L"CreateSolidColorBrush returned  : " + std::to_wstring(hr)).c_str());
		return false;
	}

	return true;
}

void Graphics::DrawMouseCoordinates(int _xpos, int _ypos)
{
	if (_xpos < 0) _xpos = 0;
	if (_ypos < 0) _ypos = 0;
	if (_xpos > m_clientRect.right) _xpos = m_clientRect.right;
	if (_ypos > m_clientRect.bottom) _ypos = m_clientRect.bottom;

	wchar_t output[50];
	wsprintf(output, L"x: %d, y: %d", _xpos, _ypos);
	D2D1_RECT_F outputRect = D2D1::RectF(5.0f, 5.0f, 500.0f, 20.0f);
	m_pRenderTarget->DrawText(output, wcslen(output), m_pTextFormat, outputRect, m_pBrush);
}

void Graphics::DrawTextRect(const wchar_t* _text, D2D1_RECT_F _rect)
{
	m_pRenderTarget->DrawTextW(_text, wcslen(_text), m_pTextFormat, _rect, m_pBrush);
}

void Graphics::DrawBitmap(Bitmap* _pBitmap, D2D1_RECT_F _rect)
{
	m_pRenderTarget->DrawBitmap(_pBitmap->GetBitmap(), _rect);
}

void Graphics::BeginDraw()
{
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear();
}

void Graphics::EndDraw()
{
	m_pRenderTarget->EndDraw();
}

void Graphics::Render()
{
	SceneManager::GetInst()->Render(this);
}

void Graphics::CleanupDevice()
{
	if (m_pD2DFactory) m_pD2DFactory->Release();
	if (m_pWICFactory) m_pWICFactory->Release();
	if (m_pRenderTarget) m_pRenderTarget->Release();
	CoUninitialize();
}
