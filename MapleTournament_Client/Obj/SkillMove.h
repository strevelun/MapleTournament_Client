#pragma once
#include "Skill.h"

class Player;

class SkillMove :
    public Skill
{
private:
    Player* m_pPlayer = nullptr;

public:
    SkillMove();
    virtual ~SkillMove();

    void SetPlayer(Player* _pPlayer) { m_pPlayer = _pPlayer; }

    void Update() override;
    void Render() override;
    void Reset() override;
};

