#pragma once
#include "Scene.h"
#include "../Setting.h"

class InGameScene :
    public Scene
{
public:
	InGameScene();
	virtual ~InGameScene();

	bool Init() override;

	void UseSkill(eSkillType _type);
};

