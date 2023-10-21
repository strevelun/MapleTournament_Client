#pragma once

#include <wincodec.h>
#include <d2d1.h>
#include <dwrite.h>

#include "../Defines.h"

class Bitmap;

class Graphics
{
private:
	ID2D1Factory* m_pD2DFactory = nullptr;
	IWICImagingFactory* m_pWICFactory = nullptr;
	IDWriteFactory* m_pDWriteFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;

	HWND					m_hWnd;

	RECT m_clientRect;

public:
	bool Init(HWND _hWnd);

	IWICImagingFactory* GetImagingFactory() const { return m_pWICFactory; }
	IDWriteFactory* GetDwriteFactory() const { return m_pDWriteFactory; }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }
	HWND GetHwnd() const { return m_hWnd; }

	bool CreateTextFormat(const wchar_t* _pFontName, FLOAT _fontSize, IDWriteTextFormat** _pFormat);
	bool CreateSolidColorBrush(D2D1::ColorF _color, ID2D1SolidColorBrush** _pBrush);

	void BeginDraw();
	void EndDraw();

	SINGLETON(Graphics)
};