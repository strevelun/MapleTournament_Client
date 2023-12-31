#include "Obj.h"
#include "../Bitmap.h"

Obj::Obj() :
	m_tPos{0,0}, m_bAlive(true)
{
}

Obj::Obj(const Obj& _obj)
{
	m_strName = _obj.m_strName;
	m_tPos = _obj.m_tPos;
	m_bAlive = _obj.m_bAlive;
	m_bActive = _obj.m_bActive;
}

Obj::Obj(float _xpos, float _ypos)
	: m_tPos{ _xpos, _ypos }, m_bAlive(true)
{
}

Obj::~Obj()
{
}

void Obj::SetPos(float _xpos, float _ypos)
{
	m_tPos.x = _xpos;
	m_tPos.y = _ypos;
}

void Obj::Input()
{
}

