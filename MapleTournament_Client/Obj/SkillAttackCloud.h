#pragma once
#include "Skill.h"
class SkillAttackCloud :
    public Skill
{
private:
    float m_duration = 0.0f;
    float m_passedTime = 0.0f;

public:
    SkillAttackCloud(float _duration);
    virtual ~SkillAttackCloud();

    void Update() override;
    void Render() override;
    void Reset() override;
};

