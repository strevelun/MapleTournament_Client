#pragma once

#include <winsock2.h>
#include <map>

#include "../Setting.h"
#include "../Packet/PacketHandler.h"


class Packet;

class TCPClient
{
private:
	HANDLE m_hThread;
	char						m_recvBuffer[255];
	u_short						m_packetSize = 0;
	int							m_totalSize = 0;
	
	static std::map<ePacketType, void(*)(char*)> m_mapPacketHandlerCallback;

	bool				m_bIsRunning;

public:
	TCPClient();
	~TCPClient();

	bool Init();

	void ReceivePacket();

private:
	void ProcessPacket(char* _packet);
	//static unsigned int __stdcall ThreadFunc(void* _pArgs);
};

