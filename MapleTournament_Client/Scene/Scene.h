#pragma once

// background (UIPanel)
// board (UIPanel)
// players 
// effects
// ui

#include <list>
#include <string>
#include <d2d1.h>

class Layer;
class Graphics;

class Scene
{
protected:
	std::list<Layer*> m_listLayer;

public:
	Scene();
	virtual ~Scene();

	Layer* FindLayer(const std::wstring& _layerName);
	Layer* CreateLayer(const std::wstring& _layerName, uint32_t _zOrder);

	virtual bool Init();
	void Update();
	void Render(Graphics* _pGraphics);
};

