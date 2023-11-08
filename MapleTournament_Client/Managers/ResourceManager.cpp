#include "ResourceManager.h"
#include "../Bitmap.h"
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

	std::map<std::wstring, std::vector<tAnimationFrame*>*>::iterator iter2 = m_mapAnimClipFrame.begin();
	std::map<std::wstring, std::vector<tAnimationFrame*>*>::iterator iterEnd2 = m_mapAnimClipFrame.end();

	for (; iter2 != iterEnd2; ++iter2)
	{
		for (tAnimationFrame* pFrame : *iter2->second)
		{
			delete pFrame;
		}
		delete iter2->second;
	}
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

	std::vector<tAnimationFrame*>* pVecFrame = new std::vector<tAnimationFrame*>();
	pVecFrame->resize(clipSize, nullptr);

	for (int i = 0; i < clipSize; i++) 
		(*pVecFrame)[i] = new tAnimationFrame(arr[i]);

	m_mapAnimClipFrame.insert({ animName, pVecFrame });

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
	std::map<std::wstring, std::vector<tAnimationFrame*>*>::iterator iter = m_mapAnimClipFrame.find(_justFilename);
	AnimationClip* pClip = nullptr;
	Bitmap* pBitmap = nullptr;

	pBitmap = GetBitmap(L"Resource/Sprite/" + _baseSheet + L".png");
	if (!pBitmap)
	{
		Debug::Log(L"Resource/Sprite/" + _baseSheet + L".png" + L"여는 도중 에러 발생");
		return nullptr;
	}

	if (iter != m_mapAnimClipFrame.end())
	{
		pClip = new AnimationClip(pBitmap, iter->second);
		return pClip;
	}
	
	if (!LoadAnimFile(L"Resource/Sprite/" + _baseSheet + L".png", L"Resource/Anim/" + _justFilename + L".anim")) return nullptr;

	iter = m_mapAnimClipFrame.find(_justFilename);
	pClip = new AnimationClip(pBitmap, iter->second);

	return pClip;
}
