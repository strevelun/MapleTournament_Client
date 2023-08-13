#include "SceneManager.h"
#include "../Scene/Scene.h"

#include <d2d1.h>

SceneManager* SceneManager::m_pInst = nullptr;

SceneManager::SceneManager() :
	m_pCurScene(nullptr), m_pNextScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	if (m_pCurScene) delete m_pCurScene;
}

void SceneManager::ChangeScene(Scene* _pScene)
{
	m_pNextScene = _pScene;
}

void SceneManager::CheckSceneChange()
{
	if (m_pNextScene == nullptr) return;

	if (m_pCurScene != m_pNextScene)
	{
		if (m_pCurScene)
		{
			//m_pCurScene->Cleanup();
			// delete m_pCurScene;
		}
		m_pCurScene = m_pNextScene;
		m_pCurScene->Init();
	}

	m_pNextScene = nullptr;
}

void SceneManager::Render(ID2D1HwndRenderTarget* _pRenderTarget)
{
	m_pCurScene->Render(_pRenderTarget);
}
