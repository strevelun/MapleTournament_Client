#pragma once

#include "Scene.h"

class LoginScene : public Scene
{
public:
	LoginScene();
	virtual ~LoginScene();

	bool Init() override;

public:
	void LoginButtonCallback(class UIEditText* _pEditText, const std::wstring& _str);
	void EndButtonCallback();
};

