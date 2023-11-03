#pragma once
#include "GameObj.h"
#include "../Setting.h"

#include <map>
#include <d2d1.h>


class Skill;

class Player :
    public GameObj
{
protected:
    std::wstring m_nickname;
    std::map<eSkillType, Skill*> m_mapSkill;
    int m_moveTime = 2; // √  ¥‹¿ß
    D2D1_POINT_2F					m_tDestPos;
    eSkillType              m_curSkill = eSkillType::None;

public:
    Player(const wchar_t* _pNickname);
    virtual ~Player();

    void Update() override;
    void Render() override;

    const std::wstring& GetNickname() const { return m_nickname; }
    void AddSkill(Skill* _pSkill, eSkillType _type);
    void UseSkill(eSkillType _type);

    int GetMoveTime() const { return m_moveTime; }
};

