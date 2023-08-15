#include "ResourceManager.h"
#include "../Bitmap.h"

#include <wincodec.h>
#include <d2d1.h>
#include <Windows.h>

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
}

bool ResourceManager::Init(IWICImagingFactory* _pImagingFactory, ID2D1HwndRenderTarget* _pRenderTarget)
{
	if(!m_pImagingFactory) m_pImagingFactory = _pImagingFactory;
	m_pRenderTarget = _pRenderTarget;

	return true;
}

ID2D1Bitmap* ResourceManager::LoadImageFromFile(const std::wstring& _fileNameWithPath)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;

	hr = m_pImagingFactory->CreateDecoderFromFilename(_fileNameWithPath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr)) return nullptr;

	IWICBitmapFrameDecode* pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr)) return nullptr;

	IWICFormatConverter* pConverter = nullptr;
	hr = m_pImagingFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr)) return nullptr;

	hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(hr)) return nullptr;

	ID2D1Bitmap* pBitmap;
	hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &pBitmap);
	if (FAILED(hr)) return nullptr;

	pConverter->Release();
	pFrame->Release();
	pDecoder->Release();

	return pBitmap;
}

Bitmap* ResourceManager::GetBitmap(const std::wstring& _fileNameWithPath)
{
	std::map<std::wstring, Bitmap*>::const_iterator iter = m_mapBitmap.find(_fileNameWithPath);
	if (iter != m_mapBitmap.cend())
		return iter->second;

	ID2D1Bitmap* pImage = LoadImageFromFile(_fileNameWithPath);
	if (pImage == nullptr)
	{
		OutputDebugStringW(L"LoadImageFromFile ¸®ÅÏ°ª nullptr");
		return nullptr;
	}
	Bitmap* pBitmap = new Bitmap(pImage);
	m_mapBitmap.insert(make_pair(_fileNameWithPath, pBitmap));

	return pBitmap;
}
