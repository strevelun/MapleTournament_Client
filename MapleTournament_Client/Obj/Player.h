#pragma once
#include "GameObj.h"
#include "../Setting.h"

#include <map>
#include <d2d1.h>

class InGameScene;
class Skill;

class Player :
    public GameObj
{
private:
    static constexpr float LeftRightMoveDist = 170.f;
    static constexpr float UpDownMoveDist = 100.f;

private:
    InGameScene* m_pScene = nullptr;

protected:
    eSkillType              m_curSkill = eSkillType::None;

public:
    Player(InGameScene* _pScene);
    virtual ~Player();

    void Update() override;
    void Render() override;

    //void AddSkill(Skill* _pSkill, eSkillType _type);
    void UseSkill(eSkillType _type);

private:
    void SkillEnd();
};

