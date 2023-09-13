#include "NetworkManager.h"

#include <WS2tcpip.h>
#include <Windows.h>

NetworkManager* NetworkManager::m_pInst = nullptr;

NetworkManager::NetworkManager() :
	m_hClientSocket(INVALID_SOCKET), m_servAddr{}
{
}

NetworkManager::~NetworkManager()
{
	if (m_hClientSocket != INVALID_SOCKET) closesocket(m_hClientSocket);
}

bool NetworkManager::Init(const char* _serverIP, int _serverPort)
{
	m_hClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hClientSocket == INVALID_SOCKET)
	{
		//L"Failed socket(). Error Code : %d\n", WSAGetLastError();
		OutputDebugStringA("failed socket");
		return false;
	}

	memset(&m_servAddr, 0, sizeof(m_servAddr));
	m_servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, _serverIP, &m_servAddr.sin_addr);
	m_servAddr.sin_port = htons(_serverPort);

	if (connect(m_hClientSocket, (SOCKADDR*)&m_servAddr, sizeof(m_servAddr)) == SOCKET_ERROR)
	{
		OutputDebugStringA("connect error");
		return false;
	}

	return true;
}

int NetworkManager::Receive(char* _buffer, int _bufferSize)
{
	return recv(m_hClientSocket, _buffer, _bufferSize, 0);
}

int NetworkManager::Send(char* _buffer)
{
	return send(m_hClientSocket, _buffer, *(USHORT*)_buffer, 0);
}
