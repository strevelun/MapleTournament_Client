#pragma once

#include <d2d1.h>
#include <string>

class Bitmap;
class Graphics;

class Obj
{
protected:
	std::wstring			m_strName;
	POINT					m_tPos;
	bool					m_bAlive = true; 
	bool					m_bActive = true;

public:
	Obj();
	Obj(INT _xpos, INT _ypos);
	virtual ~Obj();

	void SetName(const std::wstring& _strName) { m_strName = _strName; }
	virtual void SetPos(INT _xpos, INT _ypos);
	void SetAlive(bool _bAlive) { m_bAlive = _bAlive; }
	virtual void SetActive(bool _bActive) { m_bActive = _bActive; }

	const std::wstring& GetName() const { return m_strName; }
	int GetPosX() const { return m_tPos.x; }
	int GetPosY() const { return m_tPos.y; }
	bool IsAlive() const { return m_bAlive; }
	bool IsActive() const { return m_bActive; } // setVisible
	
	virtual void Input();
	virtual void Update() = 0;
	virtual void Render() = 0;
};