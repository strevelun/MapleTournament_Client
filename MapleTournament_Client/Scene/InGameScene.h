#pragma once
#include "Scene.h"
#include "../Setting.h"

#include <array>

class UI;
class UIPanel;

enum class eInGameState
{
	None,
	Prepare,
	Play,
	UseSkill,
	GameOver,
};

enum class eSkillState
{
	None,
	InUse,
	End,
};

class InGameScene :
    public Scene
{
private:
	static constexpr float StartTimer = 4.9f;
	static constexpr float PrepareTimer = 2.f;
	static constexpr float GameOverTimer = 2.f;

private:
	std::array<UI*, 10> m_arrTimer;
	bool m_isMyTurn = false;
	eInGameState m_eState = eInGameState::Prepare;
	eSkillState m_eSkillState = eSkillState::None;

	float m_timer = PrepareTimer;

public:
	InGameScene();
	virtual ~InGameScene();

	bool Init() override;
	void Update() override;

	void SetMyTurn(bool _isMyTurn);
	void ChangeState(eInGameState _state);
	void SetSkillState(eSkillState _state) { m_eSkillState = _state;  }

	//void SetSkillTimer(float _timer);

private:
	void UseSkill(eSkillType _type);
	void NextTurn();
	void OnItemButtonClick(eSkillType _type, UIPanel* _pPanel);
	void OnTimeout();
};

