#include "Player.h"
#include "SkillAttackCloud.h"
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
	std::map<eSkillType, Skill*>::const_iterator iter = m_mapSkill.find(_type);


	if (_type == eSkillType::LeftMove)
	{
		m_tPos.x -= 170;
	}
	else if (_type == eSkillType::LeftDoubleMove)
	{
		m_tPos.x -= 340;
	}
	else if (_type == eSkillType::RightMove)
	{
		m_tPos.x += 170;
	}
	else if (_type == eSkillType::RightDoubleMove)
	{
		m_tPos.x += 340;
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
