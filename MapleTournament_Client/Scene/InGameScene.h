#pragma once
#include "Scene.h"
class InGameScene :
    public Scene
{
public:
	InGameScene();
	virtual ~InGameScene();

	bool Init() override;
};

