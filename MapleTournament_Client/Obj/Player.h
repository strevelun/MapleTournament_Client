#pragma once
#include "GameObj.h"
#include "../Setting.h"

#include <map>


class Skill;

class Player :
    public GameObj
{
protected:
    std::wstring m_nickname;
    std::map<eSkillType, Skill*> m_mapSkill;

public:
    Player(const wchar_t* _pNickname);
    virtual ~Player();

    void Update() override;
    void Render() override;

    const std::wstring& GetNickname() const { return m_nickname; }
    void AddSkill(Skill* _pSkill, eSkillType _type);
    void UseSkill(eSkillType _type);
};

