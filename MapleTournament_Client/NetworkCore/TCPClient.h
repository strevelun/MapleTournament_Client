#pragma once

#include "../Setting.h"
#include "../Packet/PacketHandler.h"

class Packet;

class TCPClient
{
private:
	HANDLE m_hThread;

	PacketHandler		m_packetHandler;

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

