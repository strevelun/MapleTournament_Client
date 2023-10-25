#pragma once
#include "Scene.h"

class UIEditText;

class LobbyScene :
    public Scene
{
	float m_frameTime = 0.f;

public:
	LobbyScene();
	virtual ~LobbyScene();

	bool Init() override;

	bool ShowLobbyUI();
	bool ShowWaitingRoomUI();

	bool HideLobbyUI();
	bool HideWaitingRoomUI();

	void GameStartCallback();
	void GameReadyCallback();

	void Update() override;

private:
	bool InitLobbyUI();
	bool InitWaitingRoomUI();

	void CreateRoomButtonCallback(UIEditText* _pEditText, const std::wstring& _str);
	void ChatCallback(UIEditText* _pEditText, const std::wstring& _str);
};

