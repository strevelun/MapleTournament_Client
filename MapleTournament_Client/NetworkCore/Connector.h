#pragma once

#include "../Setting.h"

class Connector
{
private:
	SOCKADDR_IN				m_servAddr;
	SOCKET					m_hClientSocket;

public:
	Connector(const char* _serverIP, int _serverPort);
	~Connector();

	int Receive(char* _buffer, int _bufferSize);
	int Send(class Packet* _packet);
};

