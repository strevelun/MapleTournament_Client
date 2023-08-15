#pragma once

#include <wincodec.h>
#include <d2d1.h>
#include <dwrite.h>

class Bitmap;

class Graphics
{
private:
	ID2D1Factory* m_pD2DFactory = nullptr;
	IWICImagingFactory* m_pWICFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* m_pBrush = nullptr;
	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pTextFormat = nullptr;

	HWND					m_hWnd;

	RECT m_clientRect;

public:
	Graphics(HWND _hWnd);
	~Graphics();

	bool Init();

	IWICImagingFactory* GetImagingFactory() const { return m_pWICFactory; }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

	void DrawMouseCoordinates(int _xpos, int _ypos);
	void DrawTextRect(const wchar_t* _text, D2D1_RECT_F _rect);
	void DrawBitmap(Bitmap* _pBitmap, D2D1_RECT_F _rect);

	void BeginDraw();
	void EndDraw();
	void Render();

	void CleanupDevice();
};