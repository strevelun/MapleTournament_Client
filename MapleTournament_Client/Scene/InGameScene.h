#pragma once
#include "Scene.h"
#include "../Setting.h"

#include <array>

class UI;
class UIPanel;

class InGameScene :
    public Scene
{
private:
	static constexpr float StartTimer = 9.9f;
	static constexpr float PrepareTimer = 2.f;
	static constexpr float GameOverTimer = 4.f;

private:
	std::array<UI*, 10> m_arrTimer ;
	eInGameState m_eState = eInGameState::None;
	eSkillState m_eSkillState = eSkillState::None;

	float m_timer = PrepareTimer;

public:
	InGameScene();
	virtual ~InGameScene();

	bool Init() override;
	void Update() override;

	void ChangeState(eInGameState _state);
	void SetSkillState(eSkillState _state);

	void UpdateMPUI(int _slot, int _mp);
	void UpdateHPUI(int _slot, int _hp);

private:
	void SendActionPacket(eActionType _type, eMoveName _name);
	void SendActionPacket(eActionType _type, eSkillName _name);
	void NextTurn();
	void OnItemButtonClick(UIPanel* _pPanel);
	void SendExitPacket();

	void CheckAttackResult(bool& _hit, bool& _dead);
	void UpdateHeal();
	void TurnOver();
	void GameOver();

	void ShowGameOverUI();

	void UpdateDashboardUI();
};

