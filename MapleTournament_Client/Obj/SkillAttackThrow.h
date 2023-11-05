#pragma once
#include "Skill.h"
class SkillAttackThrow :
    public Skill
{
private:

public:
    SkillAttackThrow();
    virtual ~SkillAttackThrow();

    void Update() override;
    void Render() override;
    void Reset() override;
};

