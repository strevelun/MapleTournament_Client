#pragma once

#include <list>
#include <string>
#include <d2d1.h>

class Obj;
class Graphics;

class Layer
{
	std::list<Obj*>		m_listObj;

	std::wstring		m_layerName;
	uint32_t			m_zOrder;

public:
	Layer(const std::wstring& _layerName, uint32_t _zOrder);
	~Layer();

	void AddObj(Obj* _pObj);

	const std::wstring& GetName() const { return m_layerName;}
	void Update();
	void Render(Graphics* _pGraphics);
};