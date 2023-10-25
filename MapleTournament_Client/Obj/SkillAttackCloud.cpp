#include "SkillAttackCloud.h"
#include "../Timer.h"

SkillAttackCloud::SkillAttackCloud(float _duration)
	: m_duration(_duration)
{
}

SkillAttackCloud::~SkillAttackCloud()
{
}

void SkillAttackCloud::Update()
{
	Skill::Update();

	m_passedTime += Timer::GetInst()->GetDeltaTime();
	if (m_passedTime >= m_duration)
	{
		m_bActive = false;
		return;
	}

	SetPos(m_tPos.x - m_moveSpeed, m_tPos.y);
}

void SkillAttackCloud::Render()
{
	Skill::Render();
}

void SkillAttackCloud::Reset()
{
	m_passedTime = 0.0f;
}
