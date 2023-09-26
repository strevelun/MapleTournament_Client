#include "Graphics.h"
#include "../Window.h"
#include "../Managers/InputManager.h"
#include "../Bitmap.h"
#include "../Debug.h"

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
		Debug::Log((L"D2D1CreateFactory returned : " + std::to_wstring(hr)));
		return false;
	}

	GetClientRect(m_hWnd, &m_clientRect);
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE),
		D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(m_clientRect.right - m_clientRect.left, m_clientRect.bottom - m_clientRect.top)),
		&m_pRenderTarget);
	if (FAILED(hr))
	{
		Debug::Log((L"CreateHwndRenderTarget returned : " + std::to_wstring(hr)));
		return false;
	}

	hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		Debug::Log((L"CoInitialize returned : " + std::to_wstring(hr)));
		return false;
	}

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	if (FAILED(hr))
	{
		Debug::Log((L"CoCreateInstance returned  : " + std::to_wstring(hr)));
		return false;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (FAILED(hr))
	{
		Debug::Log((L"DWriteCreateFactory returned  : " + std::to_wstring(hr)));
		return false;
	}

	hr = m_pDWriteFactory->CreateTextFormat(L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		25.0f, L"en-US", &m_pTextFormatLarge);
	if (FAILED(hr))
	{
		Debug::Log((L"CreateTextFormat returned : " + std::to_wstring(hr)));
		return false;
	}

	hr = m_pDWriteFactory->CreateTextFormat(L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		15.0f, L"en-US", &m_pTextFormatSmall);
	if (FAILED(hr))
	{
		Debug::Log((L"CreateTextFormat returned : " + std::to_wstring(hr)));
		return false;
	}

	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
	if (FAILED(hr))
	{
		Debug::Log((L"CreateSolidColorBrush returned  : " + std::to_wstring(hr)));
		return false;
	}

	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBlueBrush);
	if (FAILED(hr))
	{
		Debug::Log((L"CreateSolidColorBrush returned  : " + std::to_wstring(hr)));
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
	m_pRenderTarget->DrawText(output, wcslen(output), m_pTextFormatSmall, outputRect, m_pBlackBrush);
}

void Graphics::DrawTextRectLarge(const wchar_t* _text, const D2D1_RECT_F& _rect)
{
	m_pRenderTarget->DrawTextW(_text, wcslen(_text), m_pTextFormatLarge, _rect, m_pBlackBrush);
}

void Graphics::DrawTextRectSmall(const wchar_t* _text, const D2D1_RECT_F& _rect)
{
	m_pRenderTarget->DrawTextW(_text, wcslen(_text), m_pTextFormatSmall, _rect, m_pBlackBrush);
}

void Graphics::DrawBitmap(Bitmap* _pBitmap, const D2D1_RECT_F& _srcRect)
{
	m_pRenderTarget->DrawBitmap(_pBitmap->GetBitmap(), _srcRect);
}

void Graphics::DrawBitmap(ID2D1Bitmap* _pBitmap, const D2D1_RECT_F& _destRect, const D2D1_RECT_F& _srcRect)
{
	m_pRenderTarget->DrawBitmap(_pBitmap, _destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, _srcRect);
}

void Graphics::DrawRectangle(const D2D1_RECT_F& _rect, eColor _color, int _strokeWidth)
{
	ID2D1SolidColorBrush* pBrush = nullptr;
	switch (_color)
	{
	case eColor::Black:
		pBrush = m_pBlackBrush;
		break;
	case eColor::Blue:
		pBrush = m_pBlueBrush;
		break;
	default:
		return;
	}
	m_pRenderTarget->DrawRectangle(_rect, pBrush, _strokeWidth);
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

void Graphics::CleanupDevice()
{
	// TODO : 순서반대
	if (m_pBlueBrush) m_pBlueBrush->Release();
	if (m_pBlackBrush) m_pBlackBrush->Release();
	if (m_pTextFormatSmall) m_pTextFormatSmall->Release();
	if (m_pTextFormatLarge) m_pTextFormatLarge->Release();
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
	if (m_pWICFactory) m_pWICFactory->Release();
	if (m_pRenderTarget) m_pRenderTarget->Release();
	if (m_pD2DFactory) m_pD2DFactory->Release();
	CoUninitialize();
}
