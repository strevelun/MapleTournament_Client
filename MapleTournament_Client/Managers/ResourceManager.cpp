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

bool ResourceManager::LoadImageFromFile(const std::wstring& _path, const std::wstring& _fileName)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;
	std::wstring fileNameWithPath = _path + _fileName + L".png";

	hr = m_pImagingFactory->CreateDecoderFromFilename(fileNameWithPath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
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
	hr = m_pImagingFactory->CreateFormatConverter(&pConverter);
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

	ID2D1Bitmap* pD2dBitmap;
	hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &pD2dBitmap);
	if (FAILED(hr))
	{
		Debug::Log(L"LoadImageFromFile에서 m_pRenderTarget->CreateBitmapFromWicBitmap호출 도중 오류발생. hr : " + hr);
		return false;
	}

	pConverter->Release();
	pFrame->Release();
	pDecoder->Release();

	Bitmap* pBitmap = new Bitmap(pD2dBitmap);
	m_mapBitmap.insert(make_pair(_fileName, pBitmap));

	return true;
}

bool ResourceManager::LoadAnimFile(const std::wstring& _path, const std::wstring& _fileName)
{
	std::filesystem::path filePath(_path + _fileName + L".anim");

	if (!std::filesystem::exists(filePath))
	{
		Debug::Log(_fileName + L".anim 가 존재하지 않습니다!");
		return false;
	}

	std::ifstream animFile(filePath, std::ios_base::binary);
	if (!animFile.is_open())
	{
		Debug::Log(_fileName + L".anim 여는 도중 에러 발생");
		return false;
	}

	D2D1_BITMAP_PROPERTIES bpp;
	bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bpp.dpiX = (FLOAT)0;
	bpp.dpiY = (FLOAT)0;

	int clipSize = 0;

	TCHAR animName[100];
	animFile.read((char*)&animName, 100);
	animFile.read((char*)&clipSize, sizeof(int));

	D2D1_SIZE_F bitmapSize;
	animFile.read((char*)&bitmapSize, sizeof(D2D1_SIZE_F));

	Bitmap* pBitmap = new Bitmap();

	DWORD* pixel = new DWORD[sizeof(DWORD) * bitmapSize.width * bitmapSize.height];
	animFile.read((char*)pixel, sizeof(DWORD) * bitmapSize.width * bitmapSize.height);

	ID2D1Bitmap* d2dBitmap;
	m_pRenderTarget->CreateBitmap(D2D1::SizeU(bitmapSize.width, bitmapSize.height), pixel, bitmapSize.width * 4, &bpp,&d2dBitmap);

	delete[] pixel;

	tSpriteData* arr = new tSpriteData[clipSize];
	animFile.read((char*)arr, sizeof(tSpriteData) * clipSize);

	AnimationClip* animClip = new AnimationClip(animName, clipSize);

	for (int i = 0; i < clipSize; i++) 
	{
		arr[i].bitmap = d2dBitmap;
		tAnimationFrame* frame = new tAnimationFrame(arr[i]);
		animClip->AddFrame(frame);
	}

	m_mapAnimClip.insert(std::make_pair(_fileName, animClip));

	delete[] arr;
	return true;
}

Bitmap* ResourceManager::GetBitmap(const std::wstring& _fileName)
{
	std::map<std::wstring, Bitmap*>::const_iterator iter = m_mapBitmap.find(_fileName);
	if (iter != m_mapBitmap.cend())
		return iter->second;

	if (!LoadImageFromFile(L"Resource\\UI\\", _fileName)) return nullptr;

	return m_mapBitmap.find(_fileName)->second;
}

AnimationClip* ResourceManager::GetAnimClip(const std::wstring& _fileName)
{
	std::map<std::wstring, AnimationClip*>::const_iterator iter = m_mapAnimClip.find(_fileName);
	if (iter != m_mapAnimClip.cend())
		return iter->second;
	
	if (!LoadAnimFile(L"Resource\\Anim\\", _fileName)) return nullptr;

	return m_mapAnimClip.find(_fileName)->second;
}
