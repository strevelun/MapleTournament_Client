#pragma once

// background (UIPanel)
// board (UIPanel)
// players 
// effects
// ui

#include <vector>
#include <string>
#include <d2d1.h>

class Layer;
class Graphics;

class Scene
{
private:
	std::vector<Layer*> m_vecObjLayer;

	bool m_bLoading = false;

public:
	Scene();
	virtual ~Scene();

	Layer* FindLayer(const std::wstring& _layerName);
	Layer* CreateLayer(const std::wstring& _layerName, uint32_t _zOrder, bool _isActive = true);

	static bool SortLayer(const Layer* a, const Layer* b);

	virtual bool Init();
	virtual void Update();
	void Render();
};

