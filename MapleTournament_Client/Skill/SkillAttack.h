#pragma once

#include <vector>

#include "SkillInfo.h"

class SkillAttack :
    public SkillInfo
{
private:
    friend class SkillManager;

private:
    std::vector<std::pair<int, int>> m_listCoordinates;
    int m_strikePower = 0;
    bool m_inversed = false;

public:
    SkillAttack(eSkillType _eType, int _mana);
    virtual ~SkillAttack();

    const std::vector<std::pair<int, int>>& GetListCoordinates() const { return m_listCoordinates; }
    int GetStrikePower() const { return m_strikePower; }

    bool IsInversed() const { return m_inversed; }
};

