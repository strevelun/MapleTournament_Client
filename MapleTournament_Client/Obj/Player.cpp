#include "Player.h"
#include "SkillAttackThrow.h"
#include "SkillAttackMagic.h"
#include "../Defines.h"
#include "../Animation/Animator.h"
#include "../Managers/ObjectManager.h"
#include "../Scene/InGameScene.h"
#include "../Timer.h"

Player::Player(InGameScene* _pScene) :
	m_pScene(_pScene)
{
}

Player::~Player()
{
}

void Player::Update()
{
	GameObj::Update();

	m_time = Timer::GetInst()->GetDeltaTime();

	if (m_curSkill == eSkillType::LeftMove)
	{
		m_tPos.x -= LeftRightMoveDist * m_moveSpeed * m_time;
		if (m_tPos.x <= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd();
		}
	}
	else if (m_curSkill == eSkillType::LeftDoubleMove)
	{
		m_tPos.x -= LeftRightMoveDist * 2 * m_moveSpeed * m_time;
		if (m_tPos.x <= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd();
		}
	}
	else if (m_curSkill == eSkillType::RightMove)
	{
		m_tPos.x += LeftRightMoveDist * m_moveSpeed * m_time;
		if (m_tPos.x >= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd();
		}
	}
	else if (m_curSkill == eSkillType::RightDoubleMove)
	{
		m_tPos.x += LeftRightMoveDist * 2 * m_moveSpeed * m_time ;
		if (m_tPos.x >= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd();
		}
	}
	else if (m_curSkill == eSkillType::UpMove)
	{
		m_tPos.y -= UpDownMoveDist * m_moveSpeed * m_time;
		if (m_tPos.y <= m_tDestPos.y)
		{
			m_tPos.y = m_tDestPos.y;
			SkillEnd();
		}
	}
	else if (m_curSkill == eSkillType::DownMove)
	{
		m_tPos.y += UpDownMoveDist * m_moveSpeed * m_time;
		if (m_tPos.y >= m_tDestPos.y)
		{
			m_tPos.y = m_tDestPos.y;
			SkillEnd();
		}
	}
}

void Player::Render()
{
	GameObj::Render();
}
/*
void Player::AddSkill(Skill* _pSkill, eSkillType _type)
{
	m_mapSkill.insert(std::make_pair(_type, _pSkill));
}
*/

void Player::UseSkill(eSkillType _type)
{
	if (_type == eSkillType::None) return;

	m_curSkill = _type;
	if(_type == eSkillType::LeftMove || _type == eSkillType::LeftDoubleMove || _type == eSkillType::RightMove || _type == eSkillType::RightDoubleMove
		 || _type == eSkillType::UpMove || _type == eSkillType::DownMove)
		m_pAnimator->SetNextClip(L"Walk");

	if (_type == eSkillType::LeftMove)
	{
		m_tDestPos.x = m_tPos.x - LeftRightMoveDist;
	}
	else if (_type == eSkillType::LeftDoubleMove)
	{
		m_tDestPos.x = m_tPos.x - LeftRightMoveDist * 2;
	}
	else if (_type == eSkillType::RightMove)
	{
		m_tDestPos.x = m_tPos.x + LeftRightMoveDist;
	}
	else if (_type == eSkillType::RightDoubleMove)
	{
		m_tDestPos.x = m_tPos.x + LeftRightMoveDist * 2;
	}
	else if (_type == eSkillType::UpMove)
	{
		m_tDestPos.y = m_tPos.y - UpDownMoveDist;
	}
	else if (_type == eSkillType::DownMove)
	{
		m_tDestPos.y = m_tPos.y + UpDownMoveDist;
	}
	else
	{
		Skill* pSkill = ObjectManager::GetInst()->FindSkill(_type);
		if (pSkill)
		{
			pSkill->Reset();
			pSkill->SetActive(true);
			pSkill->SetPos(m_tPos.x, m_tPos.y);
			pSkill->SetDir(m_eDir);
		}
	}
}

void Player::SkillEnd()
{
	m_curSkill = eSkillType::None;
	m_pAnimator->SetDefaultClip();
	m_pScene->SetSkillState(eSkillState::End);
}
