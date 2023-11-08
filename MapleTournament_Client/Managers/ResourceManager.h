#pragma once

#include <map>
#include <string>
#include <vector>

#include "../Defines.h"
#include "../Setting.h"

class Bitmap;
class AnimationClip;
struct ID2D1HwndRenderTarget;
struct IWICImagingFactory;
struct ID2D1Bitmap;

class ResourceManager
{
private:
	std::map<std::wstring, Bitmap*> m_mapBitmap;
	//std::map<std::wstring, AnimationClip*> m_mapAnimClip;
	std::map<std::wstring, std::vector<tAnimationFrame*>*> m_mapAnimClipFrame;

public:	
	bool Init();

	Bitmap* GetBitmap(const std::wstring& _fileWithPath);
	AnimationClip* GetAnimClip(const std::wstring& _justFilename, const std::wstring& _baseSheet);

private:
	bool LoadImageFromFile(const std::wstring& _fileWithPath);
	bool LoadAnimFile(const std::wstring& _spriteFileWithPath, const std::wstring& _animFileWithPath);

	SINGLETON(ResourceManager)
};

