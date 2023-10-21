#include "Graphics.h"
#include "../Window.h"
#include "../Managers/InputManager.h"
#include "../Bitmap.h"
#include "../Debug.h"

#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwrite.lib")

Graphics* Graphics::m_pInst = nullptr;

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
	if (m_pWICFactory) m_pWICFactory->Release();
	if (m_pRenderTarget) m_pRenderTarget->Release();
	if (m_pD2DFactory) m_pD2DFactory->Release();
	CoUninitialize();
}

bool Graphics::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	if (FAILED(hr))
	{
		Debug::Log(("D2D1CreateFactory returned : " + std::to_string(hr)));
		return false;
	}

	GetClientRect(_hWnd, &m_clientRect);
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE),
		D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(m_clientRect.right - m_clientRect.left, m_clientRect.bottom - m_clientRect.top)),
		&m_pRenderTarget);
	if (FAILED(hr))
	{
		Debug::Log(("CreateHwndRenderTarget returned : " + std::to_string(hr)));
		return false;
	}

	hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		Debug::Log(("CoInitialize returned : " + std::to_string(hr)));
		return false;
	}

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	if (FAILED(hr))
	{
		Debug::Log(("CoCreateInstance returned  : " + std::to_string(hr)));
		return false;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (FAILED(hr))
	{
		Debug::Log(("DWriteCreateFactory returned  : " + std::to_string(hr)));
		return false;
	}
	return true;
}

bool Graphics::CreateTextFormat(const wchar_t* _pFontName, FLOAT _fontSize, IDWriteTextFormat** _pFormat)
{
	HRESULT hr = m_pDWriteFactory->CreateTextFormat(_pFontName, nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		_fontSize, L"en-US", _pFormat);
	if (FAILED(hr))
	{
		Debug::Log(("CreateTextFormat returned : " + std::to_string(hr)));
		return false;
	}
	return true;
}

bool Graphics::CreateSolidColorBrush(D2D1::ColorF _color, ID2D1SolidColorBrush** _pBrush)
{
	HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_color), _pBrush);
	if (FAILED(hr))
	{
		Debug::Log(("CreateSolidColorBrush returned  : " + std::to_string(hr)));
		return false;
	}
	return true;
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