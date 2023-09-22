#pragma once

#include "../Setting.h"

class PacketHandler
{
public:
	static void S_Exit(char* _packet);
	static void S_OKLogin(char* _packet);
	static void S_FailedLogin(char* _packet);
	static void S_CreateRoom(char* _packet);
	static void S_NotifyCreateRoom(char* _packet);
	static void S_SendSessions(char* _packet);
	static void S_SendRooms(char* _packet);
	static void S_EnterOtherUser(char* _packet);
	static void S_Chat(char* _packet);
	static void S_JoinRoom(char* _packet);
	static void S_JoinRoomFail(char* _packet);
	static void S_NotifyJoinedUser(char* _packet);
	static void S_CheckRoomReadyOK(char* _packet);
	static void S_CheckRoomReadyFail(char* _packet);
	static void S_EnterLobby(char* _packet);
	static void S_LeaveRoom(char* _packet);
	static void S_UpdateRoomMemberLeave(char* _packet);
	static void S_UpdateLobbyRoomList(char* _packet);
	static void S_UpdateLobbyRoomMemberCount(char* _packet);
	static void S_UpdateUserState(char* _packet);
	static void S_UpdateUserType(char* _packet);
	static void S_UpdateWaitingRoomBtn(char* _packet);
};

