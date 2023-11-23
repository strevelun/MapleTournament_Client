#pragma once
#include "SkillInfo.h"
class SkillHeal :
    public SkillInfo
{
private:
    friend class SkillManager;

private:
    int m_heal = 0;

public:
    SkillHeal(eSkillType _eType, int _mana);
    virtual ~SkillHeal();

    int GetHeal() const { return m_heal; }
};

