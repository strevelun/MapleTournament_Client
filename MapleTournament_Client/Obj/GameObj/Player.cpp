#include "Player.h"
#include "Skill.h"
#include "../../Defines.h"
#include "../../Animation/Animator.h"
#include "../../Animation/AnimationClip.h"
#include "../../Managers/ObjectManager.h"
#include "../../Scene/InGameScene.h"
#include "../../Timer.h"
#include "../UI/UIText.h"
#include "../UI/UIPanel.h"
#include "../../Managers/UIManager.h"
#include "../../Managers/ResourceManager.h"
#include "../../Debug.h"
#include "../../Game.h"

#include <string>

Player::Player(InGameScene* _pScene, int _slot) :
	m_pScene(_pScene), m_slot(_slot)
{
	if (_slot == 1 || _slot == 3)
		m_nicknamePanelYPos = 55;
	else
		m_nicknamePanelYPos = 30;
}

Player::~Player()
{
	m_pNicknameText->SetActive(false);
}

void Player::SetPos(float _xpos, float _ypos)
{
	Obj::SetPos(_xpos, _ypos);
	if(m_pNicknameText)
		m_pNicknameText->SetPos(_xpos, _ypos + m_nicknamePanelYPos);
}

void Player::SetBoardPos(int _xpos, int _ypos)
{
	m_boardXPos = _xpos;
	m_boardYPos = _ypos;
}

void Player::Update()
{
	GameObj::Update();

	if (m_eCurActionType == eActionType::None) return;


	if (m_eCurActionType == eActionType::Move)
	{
		m_time = Timer::GetInst()->GetDeltaTime();

		if (m_eMoveName == eMoveName::LeftMove)
		{
			m_tPos.x -= LeftRightMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y + m_nicknamePanelYPos);
			if (m_tPos.x <= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::LeftDoubleMove)
		{
			m_tPos.x -= LeftRightMoveDist * 2 * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y + m_nicknamePanelYPos);
			if (m_tPos.x <= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::RightMove)
		{
			m_tPos.x += LeftRightMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y + m_nicknamePanelYPos);
			if (m_tPos.x >= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::RightDoubleMove)
		{
			m_tPos.x += LeftRightMoveDist * 2 * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y + m_nicknamePanelYPos);
			if (m_tPos.x >= m_tDestPos.x)
			{
				m_tPos.x = m_tDestPos.x;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::UpMove)
		{
			m_tPos.y -= UpDownMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y + m_nicknamePanelYPos);
			if (m_tPos.y <= m_tDestPos.y)
			{
				m_tPos.y = m_tDestPos.y;
				SkillEnd(eSkillState::End);
			}
		}
		else if (m_eMoveName == eMoveName::DownMove)
		{
			m_tPos.y += UpDownMoveDist * m_moveSpeed * m_time;
			m_pNicknameText->SetPos(m_tPos.x, m_tPos.y + m_nicknamePanelYPos);
			if (m_tPos.y >= m_tDestPos.y)
			{
				m_tPos.y = m_tDestPos.y;
				SkillEnd(eSkillState::End);
			}
		}
		
		//UI* pUI = m_pNicknameText->FindChildUI(L"text");
		//static_cast<UIText*>(pUI)->ReassignText(m_strNickname + L" (" + std::to_wstring(m_boardXPos) + L", " + std::to_wstring(m_boardYPos) + L")");

	}
	else if (m_eCurActionType == eActionType::Hit)
	{
		if (m_pAnimator->GetCurClip()->IsEnd())
		{
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurActionType == eActionType::Die)
	{
		if (m_pAnimator->GetCurClip()->IsEnd())
		{
			SkillEnd(eSkillState::End);
			m_bAlive = false;
			Debug::Log("DIE!!!");
		}
	}
	else if(m_eCurActionType == eActionType::Skill)
	{
		if (m_pCurSkill && m_pCurSkill->IsEnd())
		{
			if (m_eSkillName == eSkillName::Heal0)
				SkillEnd(eSkillState::CheckHeal);
			else
				SkillEnd(eSkillState::CheckHit);
			m_pCurSkill->Reset();
			m_pCurSkill = nullptr;
		}

	}
}

void Player::Render()
{
	GameObj::Render();
}

void Player::DoAction(eMoveName _name)
{
	if (_name == eMoveName::None || _name == eMoveName::NumOfMoves) return;

	m_eCurActionType = eActionType::Move;
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
		m_eCurActionType = eActionType::Skill;
		m_pCurSkill->SetActive(true);
		m_pCurSkill->SetPos(288.f + (176 * m_boardXPos), 298.f + (100 * m_boardYPos)); // 플레이어가 위치한 칸 중앙 (290,300)
		m_pCurSkill->SetDir(m_eDir); // 애니메이션 출력용
		m_eSkillName = _name;
	}
}

void Player::DoAction(eActionType _type)
{
	if (_type == eActionType::None || _type == eActionType::NumOfActions) return;

	m_eCurActionType = _type;

	switch (_type)
	{
	case eActionType::Hit:
		m_pAnimator->SetNextClip(L"Hit");
		break;
	case eActionType::Die:
		m_pAnimator->SetNextClip(L"Die");
		break;
	}

	switch (_type)
	{
	case eActionType::Hit:
	case eActionType::Die:
		Game::GetInst()->SetSlotInAction(m_slot, true);
		break;
	}
}

void Player::ChangeAnimationState(const std::wstring& _strStateName)
{
	m_pAnimator->SetNextClip(_strStateName);
}

UIPanel* Player::SetNicknameUI(std::wstring _strNickname)
{
	m_strNickname = _strNickname;
	Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_playernickname.png");
	m_pNicknameText = new UIPanel(nullptr, 150, 25, m_tPos.x, m_tPos.y + m_nicknamePanelYPos, 0.5f, 1.0f);
	m_pNicknameText->SetBitmap(pBitmap);
	m_pNicknameText->SetName(_strNickname);
	UIText* pText = new UIText(m_pNicknameText, _strNickname, 20.f, m_pNicknameText->GetWidth() / 2.f, m_pNicknameText->GetHeight() / 2.f, 0.5f, 0.5f);
	pText->SetName(L"text");
	m_pNicknameText->AddChildUI(pText);
	return m_pNicknameText;
}

void Player::SkillEnd(eSkillState _eSkillState)
{
	if(m_eCurActionType == eActionType::Hit || m_eCurActionType == eActionType::Die)
		Game::GetInst()->SetSlotInAction(m_slot, false);

	m_eCurActionType = eActionType::None;
	m_eMoveName = eMoveName::None;
	m_eSkillName = eSkillName::None;
	m_pAnimator->SetDefaultClip();

	
	
	if (_eSkillState == eSkillState::End && Game::GetInst()->GetInActionCount() > 0) return;

	m_pScene->SetSkillState(_eSkillState);
}
