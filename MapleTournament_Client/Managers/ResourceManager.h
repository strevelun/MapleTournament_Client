#pragma once

#include <map>
#include <string>

class Bitmap;
struct ID2D1HwndRenderTarget;
struct IWICImagingFactory;
struct ID2D1Bitmap;


class ResourceManager
{
private:
	static ResourceManager* m_pInst;

	std::map<std::wstring, Bitmap*> m_mapBitmap;

	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pImagingFactory;

	ResourceManager();
	~ResourceManager();

public:	
	static ResourceManager* GetInst()
	{
		if (!m_pInst) m_pInst = new ResourceManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
		m_pInst = nullptr;
	}

	bool Init(IWICImagingFactory* _pImagingFactory, ID2D1HwndRenderTarget* _pRenderTarget);

	Bitmap* GetBitmap(const std::wstring& _fileName, const std::wstring& _path);

private:
	ID2D1Bitmap* LoadImageFromFile(const std::wstring& _fileName, const std::wstring& _path);
};

