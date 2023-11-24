#include "Skill.h"
#include "../../Timer.h"
#include "../../Animation/Animator.h"
#include "../../Animation/AnimationClip.h"
#include "../../Debug.h"

Skill::Skill()
{
}

Skill::~Skill()
{
}

void Skill::Update()
{
	AnimationClip* pClip = m_pAnimator->GetCurClip();
	if (pClip->IsEnd())
		m_isEnd = true;

	GameObj::Update();
}

void Skill::Render()
{
	GameObj::Render();
}

void Skill::Reset()
{
	AnimationClip* pClip = m_pAnimator->GetCurClip();
	pClip->Reset();
	m_isEnd = false;
	m_bActive = false;
	//Debug::Log(L"Skill::Reset()");
}

void Skill::SetDir(eDir _eDir)
{
	m_eDir = _eDir;

	AnimationClip* pClip = m_pAnimator->GetCurClip();
	if (pClip && m_eDir == eDir::Right)		pClip->SetFlip(true);
	else									pClip->SetFlip(false);
}
