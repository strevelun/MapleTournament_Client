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
	GameOver,
};

class InGameScene :
    public Scene
{
private:
	static constexpr float StartTimer = 4.9f;
	static constexpr float PrepareTimer = 3.f;
	static constexpr float GameOverTimer = 3.f;

private:
	std::array<UI*, 10> m_arrTimer;
	bool m_isMyTurn = false;
	eInGameState m_eState = eInGameState::Prepare;

	float m_timer = StartTimer;

public:
	InGameScene();
	virtual ~InGameScene();

	bool Init() override;
	void Update() override;

	void SetMyTurn(bool _isMyTurn);
	void ChangeState(eInGameState _state);

private:
	void UseSkill(eSkillType _type);
	void NextTurn();
	void OnItemButtonClick(eSkillType _type, UIPanel* _pPanel);
	void OnTimeout();
};

