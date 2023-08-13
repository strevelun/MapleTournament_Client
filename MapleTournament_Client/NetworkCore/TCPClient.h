#pragma once

#include "../Setting.h"
#include "../Packet/PacketHandler.h"

class Packet;
class Connector;

class TCPClient
{
private:
	HANDLE m_hThread;

	Connector*			m_pConnector;
	PacketHandler		m_packetHandler;

	bool				m_bIsRunning;

public:
	TCPClient(const char* _serverIP, int _serverPort);
	~TCPClient();

	void Send(Packet* _pPacket); // TODO 존재이유

	unsigned int ReceivePacket();

private:
	void ProcessPacket(char* _packet);
	static unsigned int __stdcall ThreadFunc(void* _pArgs);
};

