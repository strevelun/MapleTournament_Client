#include "SkillMove.h"
#include "Player.h"

SkillMove::SkillMove()
{
}

SkillMove::~SkillMove()
{
}

void SkillMove::Update()
{
	if (m_pPlayer == nullptr) return;

	// �������� �����ϸ� m_pPlayer = nullptr;
}

void SkillMove::Render()
{
	Skill::Render();
}

void SkillMove::Reset()
{
}
