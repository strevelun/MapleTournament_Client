#pragma once
#include "Scene.h"
#include "../Setting.h"

#include <array>

class UI;

class InGameScene :
    public Scene
{
private:
	static constexpr float StartTimer = 4.9f;

private:
	std::array<UI*, 10> m_arrTimer;
	bool m_isMyTurn = false;

	float m_timer = StartTimer;

public:
	InGameScene();
	virtual ~InGameScene();

	bool Init() override;
	void Update() override;

	void SetMyTurn(bool _isMyTurn);

private:
	void UseSkill(eSkillType _type);
	void NextTurn();
	void OnItemButtonClick(eSkillType _type, UI* _pPanel);
	void OnTimeout();
};

