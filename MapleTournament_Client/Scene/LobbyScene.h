#pragma once
#include "Scene.h"
class LobbyScene :
    public Scene
{
public:
	LobbyScene();
	~LobbyScene();

	bool Init() override;
};

