#include "Obj.h"
#include "../Bitmap.h"

Obj::Obj() :
	m_tPos{0,0}, m_bAlive(true)
{
}

Obj::Obj(INT _xpos, INT _ypos)
	: m_tPos{ _xpos, _ypos }, m_bAlive(true)
{
}

Obj::~Obj()
{
}

void Obj::SetPos(INT _xpos, INT _ypos)
{
	m_tPos.x = _xpos;
	m_tPos.y = _ypos;
}

void Obj::Input()
{
}

