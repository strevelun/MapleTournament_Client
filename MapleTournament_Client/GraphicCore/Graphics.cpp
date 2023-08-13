#include "Graphics.h"
#include "../Window.h"

#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")

Graphics::Graphics(HWND _hWnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	if (FAILED(hr)) throw L"D2D1CreateFactory returned : " + std::to_wstring(hr);

	RECT rc;
	GetClientRect(_hWnd, &rc);
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE),
		D2D1::HwndRenderTargetProperties(_hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&m_pRenderTarget);
	if (FAILED(hr)) throw L"CreateHwndRenderTarget returned : " + std::to_wstring(hr);

	hr = CoInitialize(nullptr);
	if (FAILED(hr)) throw L"CoInitialize returned : " + std::to_wstring(hr);

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	if (FAILED(hr)) throw L"CoCreateInstance returned : " + std::to_wstring(hr);
}

Graphics::~Graphics()
{
}

void Graphics::CleanupDevice()
{
	if (m_pD2DFactory) m_pD2DFactory->Release();
	if (m_pWICFactory) m_pWICFactory->Release();
	if (m_pRenderTarget) m_pRenderTarget->Release();
	CoUninitialize();
}
