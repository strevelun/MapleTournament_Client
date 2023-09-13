#pragma once

#include "../Setting.h"
#include "../Packet/PacketHandler.h"

#include <winsock2.h>
#include <map>

class Packet;

class TCPClient
{
private:
	HANDLE m_hThread;
	
	static std::map<ePacketType, void(*)(char*)> m_mapPacketHandlerCallback;

	bool				m_bIsRunning;

public:
	TCPClient();
	~TCPClient();

	bool Init();

	unsigned int ReceivePacket();

private:
	void ProcessPacket(char* _packet);
	static unsigned int __stdcall ThreadFunc(void* _pArgs);
};

