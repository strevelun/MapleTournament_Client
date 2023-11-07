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

#include <string>

Player::Player(InGameScene* _pScene) :
	m_pScene(_pScene)
{
}

Player::~Player()
{
	if (m_pNicknameText) delete m_pNicknameText;
}

void Player::Update()
{
	GameObj::Update();

	if (m_eCurSkillType == eSkillType::None) return;

	m_time = Timer::GetInst()->GetDeltaTime();

	if (m_eCurSkillType == eSkillType::LeftMove)
	{
		m_tPos.x -= LeftRightMoveDist * m_moveSpeed * m_time;
		m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
		if (m_tPos.x <= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurSkillType == eSkillType::LeftDoubleMove)
	{
		m_tPos.x -= LeftRightMoveDist * 2 * m_moveSpeed * m_time;
		m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
		if (m_tPos.x <= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurSkillType == eSkillType::RightMove)
	{
		m_tPos.x += LeftRightMoveDist * m_moveSpeed * m_time;
		m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
		if (m_tPos.x >= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurSkillType == eSkillType::RightDoubleMove)
	{
		m_tPos.x += LeftRightMoveDist * 2 * m_moveSpeed * m_time ;
		m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
		if (m_tPos.x >= m_tDestPos.x)
		{
			m_tPos.x = m_tDestPos.x;
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurSkillType == eSkillType::UpMove)
	{
		m_tPos.y -= UpDownMoveDist * m_moveSpeed * m_time;
		m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
		if (m_tPos.y <= m_tDestPos.y)
		{
			m_tPos.y = m_tDestPos.y;
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurSkillType == eSkillType::DownMove)
	{
		m_tPos.y += UpDownMoveDist * m_moveSpeed * m_time;
		m_pNicknameText->SetPos(m_tPos.x, m_tPos.y - 120);
		if (m_tPos.y >= m_tDestPos.y)
		{
			m_tPos.y = m_tDestPos.y;
			SkillEnd(eSkillState::End);
		}
	}
	else if (m_eCurSkillType == eSkillType::Hit)
	{
		if (m_pAnimator->GetCurClip()->IsEnd())
		{
			SkillEnd(eSkillState::End);
		}
	}
	else
	{
		if (m_pCurSkill && m_pCurSkill->IsEnd())
		{
			SkillEnd(eSkillState::CheckHit);
			m_pCurSkill->Reset();
			m_pCurSkill = nullptr;
		}
	}
}

void Player::Render()
{
	GameObj::Render();
	m_pNicknameText->Render();
}
/*
void Player::AddSkill(Skill* _pSkill, eSkillType _type)
{
	m_mapSkill.insert(std::make_pair(_type, _pSkill));
}
*/

void Player::UseSkill(eSkillType _type)
{
	if (_type == eSkillType::None) return;

	m_eCurSkillType = _type;

	if(_type == eSkillType::LeftMove || _type == eSkillType::LeftDoubleMove || _type == eSkillType::RightMove || _type == eSkillType::RightDoubleMove
		 || _type == eSkillType::UpMove || _type == eSkillType::DownMove)
		m_pAnimator->SetNextClip(L"Walk");

	if (_type == eSkillType::LeftMove)
	{
		m_tDestPos.x = m_tPos.x - LeftRightMoveDist;
	}
	else if (_type == eSkillType::LeftDoubleMove)
	{
		m_tDestPos.x = m_tPos.x - LeftRightMoveDist * 2;
	}
	else if (_type == eSkillType::RightMove)
	{
		m_tDestPos.x = m_tPos.x + LeftRightMoveDist;
	}
	else if (_type == eSkillType::RightDoubleMove)
	{
		m_tDestPos.x = m_tPos.x + LeftRightMoveDist * 2;
	}
	else if (_type == eSkillType::UpMove)
	{
		m_tDestPos.y = m_tPos.y - UpDownMoveDist;
	}
	else if (_type == eSkillType::DownMove)
	{
		m_tDestPos.y = m_tPos.y + UpDownMoveDist;
	}
	else if (m_eCurSkillType == eSkillType::Hit)
	{
		m_pAnimator->SetNextClip(L"Hit");
	}
	else
	{
		m_pCurSkill = ObjectManager::GetInst()->FindSkill(_type);
		if (m_pCurSkill)
		{
			m_pCurSkill->SetActive(true);
			m_pCurSkill->SetPos(m_tPos.x, m_tPos.y);
			m_pCurSkill->SetDir(m_eDir); // 애니메이션 출력용
		}
	}
}

void Player::ChangeAnimationState(const std::wstring& _strStateName)
{
	m_pAnimator->SetNextClip(_strStateName);
}

void Player::SetNicknameUIText(std::wstring _strNickname)
{
	m_pNicknameText = new UIPanel(nullptr, 50, 25, m_tPos.x, m_tPos.y - 120, 0.5f, 1.0f);
	UIText* pText = new UIText(m_pNicknameText, _strNickname, 20.f, m_pNicknameText->GetWidth() / 2, m_pNicknameText->GetHeight() / 2, 0.5f, 0.5f);
	m_pNicknameText->AddChildUI(pText);
}

void Player::SkillEnd(eSkillState _eSkillState)
{
	m_eCurSkillType = eSkillType::None;
	m_pAnimator->SetDefaultClip();
	m_pScene->SetSkillState(_eSkillState);
}
