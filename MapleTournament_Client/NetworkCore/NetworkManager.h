#pragma once

#include <winsock2.h>

#include "../Defines.h"

class NetworkManager
{
private:
	SOCKADDR_IN				m_servAddr;
	SOCKET					m_hClientSocket;

public:
	bool Init(const char* _serverIP, int _serverPort);
	int Receive(char* _buffer, int _bufferSize);
	int Send(char* _buffer);

	SINGLETON(NetworkManager)
};