#pragma once

#include <map>
#include <string>

#include "../Defines.h"

class Bitmap;
class AnimationClip;
struct ID2D1HwndRenderTarget;
struct IWICImagingFactory;
struct ID2D1Bitmap;

class ResourceManager
{
private:
	std::map<std::wstring, Bitmap*> m_mapBitmap;
	std::map<std::wstring, AnimationClip*> m_mapAnimClip;

	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pImagingFactory;


public:	
	bool Init(IWICImagingFactory* _pImagingFactory, ID2D1HwndRenderTarget* _pRenderTarget);

	Bitmap* GetBitmap(const std::wstring& _fileName);
	AnimationClip* GetAnimClip(const std::wstring& _fileName);

private:
	bool LoadImageFromFile(const std::wstring& _path, const std::wstring& _fileName);
	bool LoadAnimFile(const std::wstring& _path, const std::wstring& _fileName);

	SINGLETON(ResourceManager)
};

