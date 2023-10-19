#pragma once

#include <wincodec.h>
#include <d2d1.h>
#include <dwrite.h>

class Bitmap;

enum class eColor
{
	Black,
	Blue
};

class Graphics
{
private:
	ID2D1Factory* m_pD2DFactory = nullptr;
	IWICImagingFactory* m_pWICFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pTextFormatLarge = nullptr;
	IDWriteTextFormat* m_pTextFormatSmall = nullptr;

	ID2D1SolidColorBrush* m_pBlackBrush = nullptr;
	ID2D1SolidColorBrush* m_pBlueBrush = nullptr;

	HWND					m_hWnd;

	RECT m_clientRect;

public:
	Graphics(HWND _hWnd);
	~Graphics();

	bool Init();

	IWICImagingFactory* GetImagingFactory() const { return m_pWICFactory; }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

	void DrawMouseCoordinates(int _xpos, int _ypos);
	void CreateTextRectLargeLayout(const wchar_t* _text, IDWriteTextLayout* _pLayout, POINT& _tPos, D2D1_RECT_F& _rect);
	void DrawTextRectLarge(const wchar_t* _text, const D2D1_RECT_F& _rect);
	void DrawTextRectSmall(const wchar_t* _text, const D2D1_RECT_F& _rect);
	void DrawTextLayout(const wchar_t* _text, const D2D1_RECT_F& _rect, IDWriteTextLayout* _pLayout);
	void DrawBitmap(Bitmap* _pBitmap, const D2D1_RECT_F& _srcRect);
	void DrawBitmap(ID2D1Bitmap* _pBitmap, const D2D1_RECT_F& _destRect, const D2D1_RECT_F& _srcRect);
	void DrawRectangle(const D2D1_RECT_F& _rect, eColor _color, int _strokeWidth);
	void SetTransform(const D2D1::Matrix3x2F& _rot);
	HWND GetHwnd() const { return m_hWnd; }

	void BeginDraw();
	void EndDraw();

	void CleanupDevice();
};