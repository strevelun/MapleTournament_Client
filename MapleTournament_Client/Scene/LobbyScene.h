#pragma once
#include "Scene.h"

class UIEditText;

class LobbyScene :
    public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene();

	bool Init() override;

	void CreateRoomButtonCallback(UIEditText* _pEditText, const std::wstring& _str);
	void ChatCallback(UIEditText* _pEditText, const std::wstring& _str);

	bool ShowLobbyUI();
	bool ShowWaitingRoomUI();

	bool HideLobbyUI();
	bool HideWaitingRoomUI();

	void GameStartCallback();
	void GameReadyCallback();

private:
	bool InitLobbyUI();
	bool InitWaitingRoomUI();

};

