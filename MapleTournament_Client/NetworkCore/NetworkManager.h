#pragma once

#include <winsock2.h>

class NetworkManager
{
private:
	static NetworkManager* m_pInst;

	SOCKADDR_IN				m_servAddr;
	SOCKET					m_hClientSocket;

	NetworkManager();
	~NetworkManager();

public:
	static NetworkManager* GetInst()
	{
		if (!m_pInst) m_pInst = new NetworkManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
		m_pInst = nullptr;
	}

	bool Init(const char* _serverIP, int _serverPort);
	int Receive(char* _buffer, int _bufferSize);
	int Send(char* _buffer);
};