#include "ResourceManager.h"
#include "../Bitmap.h"
#include "../Setting.h"
#include "../Debug.h"
#include "../Animation/AnimationClip.h"
#include "../GraphicCore/Graphics.h"

#include <wincodec.h>
#include <d2d1.h>
#include <Windows.h>
#include <filesystem>
#include <fstream>

ResourceManager* ResourceManager::m_pInst = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	std::map<std::wstring, Bitmap*>::iterator iter = m_mapBitmap.begin();
	std::map<std::wstring, Bitmap*>::iterator iterEnd = m_mapBitmap.end();

	for (; iter != iterEnd; ++iter)
		delete iter->second;

	std::map<std::wstring, AnimationClip*>::iterator iter2 = m_mapAnimClip.begin();
	std::map<std::wstring, AnimationClip*>::iterator iterEnd2 = m_mapAnimClip.end();

	for (; iter2 != iterEnd2; ++iter2)
		delete iter2->second;
}

bool ResourceManager::Init()
{
	return true;
}

bool ResourceManager::LoadImageFromFile(const std::wstring& _fileWithPath)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICImagingFactory* pImagingFactory = Graphics::GetInst()->GetImagingFactory();

	hr = pImagingFactory->CreateDecoderFromFilename(_fileWithPath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile에서 CreateDecoderFromFilename호출 도중 오류발생. hr : " + hr);
		return false;
	}

	IWICBitmapFrameDecode* pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile에서 GetFrame호출 도중 오류발생. hr : " + hr);
		return false;
	}

	IWICFormatConverter* pConverter = nullptr;
	hr = pImagingFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile에서 CreateFormatConverter호출 도중 오류발생. hr : " + hr);
		return false;
	}

	hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile에서 pConverter->Initialize호출 도중 오류발생. hr : " + hr);
		return false;
	}

	ID2D1Bitmap* pD2dBitmap = nullptr;
	hr = Graphics::GetInst()->GetRenderTarget()->CreateBitmapFromWicBitmap(pConverter, NULL, &pD2dBitmap);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile에서 m_pRenderTarget->CreateBitmapFromWicBitmap호출 도중 오류발생. hr : " + hr);
		return false;
	}

	pConverter->Release();
	pFrame->Release();
	pDecoder->Release();

	Bitmap* pBitmap = new Bitmap(pD2dBitmap);
	m_mapBitmap.insert(make_pair(_fileWithPath, pBitmap));

	return true;
}

bool ResourceManager::LoadAnimFile(const std::wstring& _spriteFileWithPath, const std::wstring& _animFileWithPath)
{
	if (!std::filesystem::exists(_animFileWithPath))
	{
		Debug::Log(L"ResourceManager::LoadAnimFile : " + _animFileWithPath + L"가 존재하지 않습니다!");
		return false;
	}

	std::ifstream animFile(_animFileWithPath, std::ios_base::binary);
	if (!animFile.is_open())
	{
		Debug::Log(L"ResourceManager::LoadAnimFile : " + _animFileWithPath + L"여는 도중 에러 발생");
		return false;
	}

	int clipSize = 0;	
	TCHAR animName[100];
	animFile.read((char*)&animName, 100);
	animFile.read((char*)&clipSize, sizeof(int));

	tSpriteData* arr = new tSpriteData[clipSize];
	animFile.read((char*)arr, sizeof(tSpriteData) * clipSize);

	Bitmap* pBitmap = GetBitmap(_spriteFileWithPath);
	if (!pBitmap)
	{
		Debug::Log(L"ResourceManager::LoadAnimFile : " + _spriteFileWithPath + L"여는 도중 에러 발생");
		return false;
	}

	
	AnimationClip* animClip = new AnimationClip(pBitmap, clipSize);

	for (int i = 0; i < clipSize; i++) 
	{
		tAnimationFrame* frame = new tAnimationFrame(arr[i]);
		animClip->AddFrame(frame);
	}

	m_mapAnimClip.insert(std::make_pair(animName, animClip));
	delete[] arr;

	return true;
}

Bitmap* ResourceManager::GetBitmap(const std::wstring& _fileWithPath)
{
	std::map<std::wstring, Bitmap*>::iterator iter = m_mapBitmap.find(_fileWithPath);
	if (iter != m_mapBitmap.end())
		return iter->second;

	if (!LoadImageFromFile(_fileWithPath)) return nullptr;

	return m_mapBitmap.find(_fileWithPath)->second;
}

AnimationClip* ResourceManager::GetAnimClip(const std::wstring& _justFilename, const std::wstring& _baseSheet)
{
	std::map<std::wstring, AnimationClip*>::iterator iter = m_mapAnimClip.find(_justFilename);
	AnimationClip* pClip = nullptr;

	if (iter != m_mapAnimClip.end())
	{
		pClip = new AnimationClip(*iter->second);
		return pClip;
	}
	
	if (_baseSheet == L"")
	{
		if (!LoadAnimFile(L"Resource/Sprite/" + _justFilename + L".png", L"Resource/Anim/" + _justFilename + L".anim")) return nullptr;
	}
	else
	{
		if (!LoadAnimFile(L"Resource/Sprite/" + _baseSheet + L".png", L"Resource/Anim/" + _justFilename + L".anim")) return nullptr;
	}

	pClip = new AnimationClip(*m_mapAnimClip.find(_justFilename)->second);

	return pClip;
}
