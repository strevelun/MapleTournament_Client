#pragma once

class Scene;
struct ID2D1HwndRenderTarget;

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

	void Render(ID2D1HwndRenderTarget* _pRenderTarget);
};

