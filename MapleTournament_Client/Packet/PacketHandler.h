#pragma once

#include "../Setting.h"

class PacketHandler
{
public:
	static void S_OKLogin(char* _packet);
	static void S_FailedLogin(char* _packet);
	static void S_CreateRoom(char* _packet);
	static void S_Chat(char* _packet);
	static void S_JoinRoom(char* _packet);
	static void S_JoinRoomFail(char* _packet);
	static void S_NotifyJoinedUser(char* _packet);
	static void S_CheckRoomReadyOK(char* _packet);
	static void S_CheckRoomReadyFail(char* _packet);
	static void S_EnterLobby(char* _packet);
	static void S_UpdateRoomMemberLeave(char* _packet);
	static void S_UpdateUserState(char* _packet);
	static void S_UpdateUserType(char* _packet);
	static void S_UpdateWaitingRoomBtn(char* _packet);
	static void S_InGameReady(char* _packet);
	static void S_UpdateUserListPage(char* _packet);
	static void S_UpdateRoomListPage(char* _packet);
	static void S_UpdateUserSlot(char* _packet);
	static void S_Skill(char* _packet);
	static void S_UpdateTurn(char* _packet);
	static void S_UpdateDashboard(char* _packet);
	static void S_GameOver(char* _packet);
	static void S_GameOverSceneChange(char* _packet);
	static void S_UpdateIngameUserLeave(char* _packet);
	static void S_Standby(char* _packet);
	static void S_UpdateProfile(char* _packet);
	static void S_CheckHit(char* _packet);
	static void S_UpdateHeal(char* _packet);
	static void S_CreatePortal(char* _packet);
	static void S_Teleport(char* _packet);
	static void S_ExitInGame(char* _packet);
};

