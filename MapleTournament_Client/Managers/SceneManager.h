#pragma once

#include <d2d1.h>

class Scene;
class Graphics;

class SceneManager
{
private:
	static SceneManager* m_pInst;

	Scene* m_pCurScene;
	Scene* m_pNextScene;

	SceneManager();
	~SceneManager();

public:
	static SceneManager* GetInst()
	{
		if (!m_pInst) m_pInst = new SceneManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
		m_pInst = nullptr;
	}

	void ChangeScene(Scene* _pScene);
	void CheckSceneChange();

	template <typename T>
	T* GetCurScene() { return dynamic_cast<T*>(m_pCurScene); }

	void Update();
	void Render(Graphics* _pGraphics);
};

