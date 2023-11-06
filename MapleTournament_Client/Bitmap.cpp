#include "Bitmap.h"

Bitmap::Bitmap() :
	m_pD2DBitmap(nullptr)
{
}
Bitmap::Bitmap(ID2D1Bitmap* _bitmap) :
	m_pD2DBitmap(_bitmap)
{

}

Bitmap::~Bitmap()
{
	//if (m_pD2DBitmap)
	//	m_pD2DBitmap->Release();
}
