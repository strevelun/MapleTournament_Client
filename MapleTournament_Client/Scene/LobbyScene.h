#pragma once
#include "Scene.h"
#include "../Setting.h"

class UIEditText;

class LobbyScene :
    public Scene
{
	float m_frameTime = 0.f;

	eSessionState m_state = eSessionState::Lobby;

public:
	LobbyScene();
	virtual ~LobbyScene();

	bool Init() override;

	void GameStartCallback();
	void GameReadyCallback();

	void Update() override;

	void ChangeSceneUI(eSessionState _state);

private:
	bool InitLobbyUI();
	bool InitWaitingRoomUI();

	bool ShowLobbyUI();
	bool ShowWaitingRoomUI();

	bool HideLobbyUI();
	bool HideWaitingRoomUI();

	void CreateRoomButtonCallback(UIEditText* _pEditText, const std::wstring& _str);
	void ChatCallback(UIEditText* _pEditText, const std::wstring& _str);

	void UpdateUserListPage();
	void UpdateRoomListPage();
};

