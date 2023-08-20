#pragma once

#include "Scene.h"

class LoginScene : public Scene
{
public:
	LoginScene();
	~LoginScene();

	bool Init() override;

public:
	void LoginButtonCallback(const std::wstring& _str);
	void EndButtonCallback();
};

