#pragma once

#include <d2d1.h>

class Bitmap
{
private:
	ID2D1Bitmap* m_pD2DBitmap;
	D2D1_SIZE_F m_bitmapSize;

	unsigned int m_sliceX = 1;

public:
	Bitmap();
	Bitmap(ID2D1Bitmap* _bitmap);
	~Bitmap();

	ID2D1Bitmap* GetBitmap() const { return m_pD2DBitmap; }
	float GetWidth() const { return m_bitmapSize.width; }
	float GetHeight() const { return m_bitmapSize.height; }
};

