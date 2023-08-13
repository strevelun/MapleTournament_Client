#include "Connector.h"
#include "../Packet/Packet.h"

Connector::Connector(const char* _serverIP, int _serverPort) :
	m_hClientSocket(INVALID_SOCKET)
{
	m_hClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hClientSocket == INVALID_SOCKET) throw L"Failed socket(). Error Code : %d\n", WSAGetLastError();

	memset(&m_servAddr, 0, sizeof(m_servAddr));
	m_servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, _serverIP, &m_servAddr.sin_addr);
	m_servAddr.sin_port = htons(_serverPort);

	if (connect(m_hClientSocket, (SOCKADDR*)&m_servAddr, sizeof(m_servAddr)) == SOCKET_ERROR) throw L"Connector connect error";
}

Connector::~Connector()
{
	if (m_hClientSocket != INVALID_SOCKET) closesocket(m_hClientSocket);
}

int Connector::Receive(char* _buffer, int _bufferSize)
{
	return recv(m_hClientSocket, _buffer, _bufferSize, 0);
}

int Connector::Send(Packet* _packet)
{
	const char* buffer = _packet->GetPacketBuffer();
	return send(m_hClientSocket, buffer, _packet->GetPacketSize(), 0);
}
