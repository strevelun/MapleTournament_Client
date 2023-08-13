#pragma once

#include <wincodec.h>

#include <d2d1.h>

class Graphics
{
private:
	ID2D1Factory* m_pD2DFactory = nullptr;
	IWICImagingFactory* m_pWICFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;

public:
	Graphics(HWND _hWnd);
	~Graphics();

	IWICImagingFactory* GetImagingFactory() const { return m_pWICFactory; }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

	void CleanupDevice();
};