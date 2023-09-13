#pragma once

#include "../Setting.h"

class PacketHandler
{
public:
	static void S_Exit(char* _packet);
	static void S_OKLogin(char* _packet);
	static void S_FailedLogin(char* _packet);
	static void S_CreateRoom(char* _packet);
	static void S_SendSessions(char* _packet);
	static void S_SendRooms(char* _packet);
	static void S_EnterOtherUser(char* _packet);
	static void S_Chat(char* _packet);
	static void S_JoinRoom(char* _packet);
};

