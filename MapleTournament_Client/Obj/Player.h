#pragma once
#include "GameObj.h"
#include "../Setting.h"

#include <map>
#include <d2d1.h>

class InGameScene;
class Skill;
class UIPanel;

class Player :
    public GameObj
{
public:
    static constexpr float LeftRightMoveDist = 180.f;
    static constexpr float UpDownMoveDist = 100.f;

private:
    InGameScene* m_pScene = nullptr;
    Skill* m_pCurSkill = nullptr;
    //std::wstring m_strNickname;
    UIPanel* m_pNicknameText = nullptr;

protected:
    eActionType              m_eCurSkillType = eActionType::None;
    eMoveName                   m_eMoveName = eMoveName::None;
    eSkillName                  m_eSkillName = eSkillName::None;

public:
    Player(InGameScene* _pScene);
    virtual ~Player();

    void SetPos(float _xpos, float _ypos) override;

    void Update() override;
    void Render() override;

    //void AddSkill(Skill* _pSkill, eSkillType _type);
    void DoAction(eMoveName _name);
    void DoAction(eSkillName _name);
    void DoAction(eActionType _type);
    void ChangeAnimationState(const std::wstring& _strStateName);
    void SetNicknameUIText(std::wstring _strNickname);

    eActionType GetActionType() const { return m_eCurSkillType; }
    
    void SkillEnd(eSkillState _eSkillState); // actionEnd
};

