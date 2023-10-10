#include "ResourceManager.h"
#include "../Bitmap.h"
#include "../Setting.h"
#include "../Debug.h"
#include "../Animation/AnimationClip.h"

#include <wincodec.h>
#include <d2d1.h>
#include <Windows.h>
#include <filesystem>
#include <fstream>

ResourceManager* ResourceManager::m_pInst = nullptr;

ResourceManager::ResourceManager() :
	m_pRenderTarget(nullptr),
	m_pImagingFactory(nullptr)
{
}

ResourceManager::~ResourceManager()
{
	std::map<std::wstring, Bitmap*>::iterator iter = m_mapBitmap.begin();
	std::map<std::wstring, Bitmap*>::iterator iterEnd = m_mapBitmap.end();

	for (; iter != iterEnd; iter++)
		delete iter->second;

	std::map<std::wstring, AnimationClip*>::iterator iter2 = m_mapAnimClip.begin();
	std::map<std::wstring, AnimationClip*>::iterator iterEnd2 = m_mapAnimClip.end();

	for (; iter2 != iterEnd2; iter2++)
		delete iter2->second;
}

bool ResourceManager::Init(IWICImagingFactory* _pImagingFactory, ID2D1HwndRenderTarget* _pRenderTarget)
{
	if(!m_pImagingFactory) m_pImagingFactory = _pImagingFactory;
	m_pRenderTarget = _pRenderTarget;

	return true;
}

bool ResourceManager::LoadImageFromFile(const std::wstring& _fileWithPath)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;

	hr = m_pImagingFactory->CreateDecoderFromFilename(_fileWithPath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile���� CreateDecoderFromFilenameȣ�� ���� �����߻�. hr : " + hr);
		return false;
	}

	IWICBitmapFrameDecode* pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile���� GetFrameȣ�� ���� �����߻�. hr : " + hr);
		return false;
	}

	IWICFormatConverter* pConverter = nullptr;
	hr = m_pImagingFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile���� CreateFormatConverterȣ�� ���� �����߻�. hr : " + hr);
		return false;
	}

	hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile���� pConverter->Initializeȣ�� ���� �����߻�. hr : " + hr);
		return false;
	}

	ID2D1Bitmap* pD2dBitmap;
	hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &pD2dBitmap);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile���� m_pRenderTarget->CreateBitmapFromWicBitmapȣ�� ���� �����߻�. hr : " + hr);
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
		Debug::Log(L"ResourceManager::LoadAnimFile : " + _animFileWithPath + L"�� �������� �ʽ��ϴ�!");
		return false;
	}

	std::ifstream animFile(_animFileWithPath, std::ios_base::binary);
	if (!animFile.is_open())
	{
		Debug::Log(L"ResourceManager::LoadAnimFile : " + _animFileWithPath + L"���� ���� ���� �߻�");
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
		Debug::Log(L"ResourceManager::LoadAnimFile : " + _spriteFileWithPath + L"���� ���� ���� �߻�");
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
	std::map<std::wstring, Bitmap*>::const_iterator iter = m_mapBitmap.find(_fileWithPath);
	if (iter != m_mapBitmap.cend())
		return iter->second;

	if (!LoadImageFromFile(_fileWithPath)) return nullptr;

	return m_mapBitmap.find(_fileWithPath)->second;
}

AnimationClip* ResourceManager::GetAnimClip(const std::wstring& _justFilename)
{
	std::map<std::wstring, AnimationClip*>::const_iterator iter = m_mapAnimClip.find(_justFilename);
	if (iter != m_mapAnimClip.cend())
		return iter->second;
	
	if (!LoadAnimFile(L"Resource/Sprite/" + _justFilename + L".png", L"Resource/Anim/" + _justFilename + L".anim")) return nullptr;

	return m_mapAnimClip.find(_justFilename)->second;
}
