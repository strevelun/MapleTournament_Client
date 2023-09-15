#pragma once

#include <map>
#include <string>

#include "../Defines.h"

class Bitmap;
struct ID2D1HwndRenderTarget;
struct IWICImagingFactory;
struct ID2D1Bitmap;

class ResourceManager
{
private:
	std::map<std::wstring, Bitmap*> m_mapBitmap;

	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pImagingFactory;


public:	
	bool Init(IWICImagingFactory* _pImagingFactory, ID2D1HwndRenderTarget* _pRenderTarget);

	Bitmap* GetBitmap(const std::wstring& _fileNameWithPath);

private:
	ID2D1Bitmap* LoadImageFromFile(const std::wstring& _fileNameWithPath);

	SINGLETON(ResourceManager)
};

