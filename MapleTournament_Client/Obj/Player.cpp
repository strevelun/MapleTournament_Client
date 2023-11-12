#include "Player.h"
#include "Skill.h"
#include "../Defines.h"
#include "../Animation/Animator.h"
#include "../Animation/AnimationClip.h"
#include "../Managers/ObjectManager.h"
#include "../Scene/InGameScene.h"
#include "../Timer.h"
#include "UI/UIText.h"
#include "UI/UIPanel.h"
#include "../Managers/UIManager.h"

#include <string>

Player::Player(InGameScene* _pScene) :
	m_pScene(_pScene)
{
}

Player::~Player()
{
	m_pNicknameText->SetActive(false);
}

void Player::SetPos(float _xpos, float _ypos)
{
	Obj::SetPos(_xpos, _ypos);
	if(m_pNicknameText)
		m_pNicknameText->SetPos(_xpos, _ypos - 120);
}

void Player::SetBoardPos(int _xpos, int _ypos)
{
	m_boardXPos = _xpos;
	m_boardYPos = _ypos;
}

void Player::Update()
{
	GameObj::Update();

	if (m_eCurSkillType == eActionType::None) return;

	m_time = Timer::GetInst()->GetDeltaTime();

	if (m_eCurSkillType == eActionType::Move)
	{
		if (m_eMoveName == eMoveName::LeftMove)
		{
			m_tPos.x -= LeftRightMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
			if (m_tPos.x <= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::LeftDoubleMove)
		{
			m_tPos.x -= LeftRightMoveDist * 2 * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
			if (m_tPos.x <= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::RightMove)
		{
			m_tPos.x += LeftRightMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
			if (m_tPos.x >= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::RightDoubleMove)
		{
			m_tPos.x += LeftRightMoveDist * 2 * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
			if (m_tPos.x >= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::UpMove)
		{
			m_tPos.y -= UpDownMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
			if (m_tPos.y <= m_tDestPos.y)
			{
				m_tPos.y = m_tDestPos.y;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::DownMove)
		{
			m_tPos.y += UpDownMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
			if (m_tPos.y >= m_tDestPos.y)
			{
				m_tPos.y = m_tDestPos.y;
				SkillEnd(eSkillState::End);
			}
		}
	}
	else if (m_eCurSkillType == eActionType::Hit)
	{
		if (m_pAnimator->GetCurClip()->IsEnd())
		{
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurSkillType == eActionType::Die)
	{
		if (m_pAnimator->GetCurClip()->IsEnd())
		{
			SkillEnd(eSkillState::End);
			m_bAlive = false;
			

		}
	}
	else if(m_eCurSkillType == eActionType::Skill)
	{
		if (m_pCurSkill && m_pCurSkill->IsEnd())
		{
			if (m_eSkillName == eSkillName::Heal0)
			{
				SkillEnd(eSkillState::CheckHeal);
				m_pCurSkill->Reset();
				m_pCurSkill = nullptr;
			}
			else
			{
				SkillEnd(eSkillState::CheckHit);
				m_pCurSkill->Reset();
				m_pCurSkill = nullptr;
			}
		}

	}
}

void Player::Render()
{
	GameObj::Render();
}
/*
void Player::AddSkill(Skill* _pSkill, eSkillType _type)
{
	m_mapSkill.insert(std::make_pair(_type, _pSkill));
}
*/

void Player::DoAction(eMoveName _name)
{
	if (_name == eMoveName::None || _name == eMoveName::NumOfMoves) return;

	m_eCurSkillType = eActionType::Move;
	m_pAnimator->SetNextClip(L"Walk");

	if (_name == eMoveName::LeftMove)
	{
		m_tDestPos.x = m_tPos.x - LeftRightMoveDist;
		m_boardXPos -= 1;
		m_eMoveName = _name;
	}
	else if (_name == eMoveName::LeftDoubleMove)
	{
		m_tDestPos.x = m_tPos.x - LeftRightMoveDist * 2;
		m_boardXPos -= 2;
		m_eMoveName = _name;
	}
	else if (_name == eMoveName::RightMove)
	{
		m_tDestPos.x = m_tPos.x + LeftRightMoveDist;
		m_boardXPos += 1;
		m_eMoveName = _name;
	}
	else if (_name == eMoveName::RightDoubleMove)
	{
		m_tDestPos.x = m_tPos.x + LeftRightMoveDist * 2;
		m_boardXPos += 2;
		m_eMoveName = _name;
	}
	else if (_name == eMoveName::UpMove)
	{
		m_tDestPos.y = m_tPos.y - UpDownMoveDist;
		m_boardYPos -= 1;
		m_eMoveName = _name;
	}
	else if (_name == eMoveName::DownMove)
	{
		m_tDestPos.y = m_tPos.y + UpDownMoveDist;
		m_boardYPos += 1;
		m_eMoveName = _name;
	}
}

void Player::DoAction(eSkillName _name) // hit, die
{
	if (_name == eSkillName::None || _name == eSkillName::NumOfSkills) return;

	m_pCurSkill = ObjectManager::GetInst()->FindSkill(_name);
	if (m_pCurSkill)
	{
		m_eCurSkillType = eActionType::Skill;
		m_pCurSkill->SetActive(true);
		m_pCurSkill->SetPos(288+(176* m_boardXPos), 298+(100* m_boardYPos)); // 플레이어가 위치한 칸 중앙 (290,300)
		m_pCurSkill->SetDir(m_eDir); // 애니메이션 출력용
		m_eSkillName = _name;
	}
}

void Player::DoAction(eActionType _type)
{
	if (_type == eActionType::None || _type == eActionType::NumOfActions) return;

	if (_type == eActionType::Hit)
	{
		m_eCurSkillType = _type;
		m_pAnimator->SetNextClip(L"Hit");
	}
	else if (_type == eActionType::Die)
	{
		m_eCurSkillType = _type;
		m_pAnimator->SetNextClip(L"Die");
	}
}

void Player::ChangeAnimationState(const std::wstring& _strStateName)
{
	m_pAnimator->SetNextClip(_strStateName);
}

UIPanel* Player::SetNicknameUI(std::wstring _strNickname)
{
	m_pNicknameText = new UIPanel(nullptr, 50, 25, m_tPos.x, m_tPos.y - 120, 0.5f, 1.0f);
	m_pNicknameText->SetName(_strNickname);
	UIText* pText = new UIText(m_pNicknameText, _strNickname, 20.f, m_pNicknameText->GetWidth() / 2, m_pNicknameText->GetHeight() / 2, 0.5f, 0.5f);
	m_pNicknameText->AddChildUI(pText);
	return m_pNicknameText;
}

void Player::SkillEnd(eSkillState _eSkillState)
{
	m_eCurSkillType = eActionType::None;
	m_eMoveName = eMoveName::None;
	m_eSkillName = eSkillName::None;
	m_pAnimator->SetDefaultClip();
	m_pScene->SetSkillState(_eSkillState);
}
