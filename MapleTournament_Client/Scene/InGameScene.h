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
	static constexpr float StartTimer = 6.9f;
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
	void SendActionPacket(eActionType _type, eMoveName _name);
	void SendActionPacket(eActionType _type, eSkillName _name);
	void NextTurn();
	void OnItemButtonClick(UIPanel* _pPanel);
	void OnTimeout();
};

