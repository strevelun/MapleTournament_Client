#pragma once
#include "Scene.h"
#include "../Setting.h"

class UIEditText;

class LobbyScene :
    public Scene
{
	float m_frameTime = 0.f;

	eSessionState m_state = eSessionState::Lobby;
	eSessionState m_nextState = eSessionState::None;

public:
	LobbyScene();
	virtual ~LobbyScene();

	bool Init() override;

	void GameStartCallback();
	void GameReadyCallback();

	eSessionState GetState() const { return m_state; }

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

	void UpdateUserSlot(u_int _choice);
};

