#include "Player.h"
#include "SkillAttackCloud.h"
#include "../Defines.h"
#include "../Animation/Animator.h"
#include "SkillMove.h"

Player::Player(const wchar_t* _pNickname)
	: m_nickname(_pNickname)
{
}

Player::~Player()
{
}

void Player::Update()
{
	GameObj::Update();

	if (m_curSkill == eSkillType::LeftMove)
	{
		m_tPos.x -= (170.f / (FRAME_PER_SEC * m_moveTime)) * 3;
		if (m_tPos.x <= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			m_curSkill = eSkillType::None;
			m_pAnimator->SetDefaultClip();
		}
	}
	else if (m_curSkill == eSkillType::LeftDoubleMove)
	{
		m_tPos.x -= (340.f / (FRAME_PER_SEC * m_moveTime)) * 3;
		if (m_tPos.x <= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			m_curSkill = eSkillType::None;
			m_pAnimator->SetDefaultClip();
		}
	}
	else if (m_curSkill == eSkillType::RightMove)
	{
		m_tPos.x += (170.f / (FRAME_PER_SEC * m_moveTime)) * 3;
		if (m_tPos.x >= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			m_curSkill = eSkillType::None;
			m_pAnimator->SetDefaultClip();
		}
	}
	else if (m_curSkill == eSkillType::RightDoubleMove)
	{
		m_tPos.x += (340.f / (FRAME_PER_SEC * m_moveTime)) * 3;
		if (m_tPos.x >= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			m_curSkill = eSkillType::None;
			m_pAnimator->SetDefaultClip();
		}
	}
	else if (m_curSkill == eSkillType::UpMove)
	{
		m_tPos.y -= (100.f / (FRAME_PER_SEC * m_moveTime)) * 3;
		if (m_tPos.y <= m_tDestPos.y)
		{
			m_tPos.y = m_tDestPos.y;
			m_curSkill = eSkillType::None;
			m_pAnimator->SetDefaultClip();
		}
	}
	else if (m_curSkill == eSkillType::DownMove)
	{
		m_tPos.y += (100.f / (FRAME_PER_SEC * m_moveTime)) * 3;
		if (m_tPos.y >= m_tDestPos.y)
		{
			m_tPos.y = m_tDestPos.y;
			m_curSkill = eSkillType::None;
			m_pAnimator->SetDefaultClip();
		}
	}
}

void Player::Render()
{
	GameObj::Render();
}

void Player::AddSkill(Skill* _pSkill, eSkillType _type)
{
	m_mapSkill.insert(std::make_pair(_type, _pSkill));
}

void Player::UseSkill(eSkillType _type)
{
	if (_type == eSkillType::None) return;

	std::map<eSkillType, Skill*>::const_iterator iter = m_mapSkill.find(_type);

	m_curSkill = _type;
	if(_type == eSkillType::LeftMove || _type == eSkillType::LeftDoubleMove || _type == eSkillType::RightMove || _type == eSkillType::RightDoubleMove
		 || _type == eSkillType::UpMove || _type == eSkillType::DownMove)
		m_pAnimator->SetNextClip(L"Walk");

	if (_type == eSkillType::LeftMove)
	{
		m_tDestPos.x = m_tPos.x - 170;
	}
	else if (_type == eSkillType::LeftDoubleMove)
	{
		m_tDestPos.x = m_tPos.x - 340;
	}
	else if (_type == eSkillType::RightMove)
	{
		m_tDestPos.x = m_tPos.x + 170;
	}
	else if (_type == eSkillType::RightDoubleMove)
	{
		m_tDestPos.x = m_tPos.x + 340;
	}
	else if (_type == eSkillType::UpMove)
	{
		m_tDestPos.y = m_tPos.y - 100;
	}
	else if (_type == eSkillType::DownMove)
	{
		m_tDestPos.y = m_tPos.y + 100;
	}
	else
	{
		if (iter != m_mapSkill.cend())
		{
			iter->second->Reset();
			iter->second->SetActive(true);
			iter->second->SetPos(m_tPos.x, m_tPos.y);
		}
	}
}
