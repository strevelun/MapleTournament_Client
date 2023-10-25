#include "GameObj.h"
#include "../Animation/Animator.h"
#include "../GraphicCore/Graphics.h"

GameObj::GameObj()
{
}

GameObj::~GameObj()
{
	if (m_pAnimator) delete m_pAnimator;
}

void GameObj::Update()
{
	if (m_pAnimator)
		m_pAnimator->Update();
}

void GameObj::Render()
{
	if (m_pAnimator)
	{
		m_pAnimator->Render(m_tPos.x, m_tPos.y, m_ratio);
	}
}

void GameObj::SetAnimator(Animator* _pAnim)
{
	if (m_pAnimator) delete m_pAnimator;
	m_pAnimator = _pAnim;
}
