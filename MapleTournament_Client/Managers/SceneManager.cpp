#include "SceneManager.h"
#include "../Scene/Scene.h"

SceneManager* SceneManager::m_pInst = nullptr;

SceneManager::SceneManager() :
	m_pCurScene(nullptr), m_pNextScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	if (m_pCurScene) delete m_pCurScene;
}

void SceneManager::CheckSceneChange()
{
	if (m_pNextScene == nullptr) return;

	if (m_pCurScene != m_pNextScene)
	{
		if (m_pCurScene)
		{
			delete m_pCurScene;
		}
		m_pCurScene = m_pNextScene;
		m_pCurScene->Init();
		
	}

	m_pNextScene = nullptr;
}


void SceneManager::Update()
{
	m_pCurScene->Update();
}

void SceneManager::Render()
{
	m_pCurScene->Render();
}
