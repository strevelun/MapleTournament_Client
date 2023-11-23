#pragma once

#include "../Setting.h"

class SkillInfo
{
protected:
	eSkillType		m_eType = eSkillType::None;
	int				m_mana = 0;

	SkillInfo(eSkillType _eType, int _mana);
	virtual ~SkillInfo();

public:
	eSkillType GetType() const { return m_eType; }
	int GetMana() const { return m_mana; }
};

