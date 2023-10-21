#pragma once

#include <d2d1.h>

#include "../Defines.h"

class Scene;
class Graphics;

class SceneManager
{
private:
	Scene* m_pCurScene;
	Scene* m_pNextScene;

public:

	void ChangeScene(Scene* _pScene) { m_pNextScene = _pScene; }
	void CheckSceneChange();

	template <typename T>
	T* GetCurScene() { return dynamic_cast<T*>(m_pCurScene); }

	void Update();
	void Render();

	SINGLETON(SceneManager)
};

