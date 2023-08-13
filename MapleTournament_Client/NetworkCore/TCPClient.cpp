#include "Connector.h"
#include "TCPClient.h"
#include "../Packet/Packet.h"
#include "../Packet/EnterPacket.h"

#include <thread>

TCPClient::TCPClient(const char* _serverIP, int _serverPort)
	: m_bIsRunning(true)
{
	m_pConnector = new Connector(_serverIP, _serverPort);
	
	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, 0, &threadID);
	if (m_hThread == NULL) throw L"Failed BeginThreadEx";
}

TCPClient::~TCPClient()
{
    CloseHandle(m_hThread);

	if (m_pConnector) delete m_pConnector;
}

void TCPClient::Send(Packet* _pPacket) 
{
	m_pConnector->Send(_pPacket);
}

unsigned int TCPClient::ReceivePacket()
{
	int							recvSize, curTotalRecvSize = 0;
	u_short						packetSize;
	char						recvBuffer[255];
	int bufferSize = sizeof(recvBuffer);

	bool isRunning = true;

	while (isRunning)
	{
		recvSize = m_pConnector->Receive(recvBuffer + curTotalRecvSize, bufferSize - curTotalRecvSize);
		if (recvSize == -1) break;

		curTotalRecvSize += recvSize;

		while (1)
		{
			packetSize = *(u_short*)recvBuffer;
			if (curTotalRecvSize < 2 || packetSize > curTotalRecvSize) break;

			ProcessPacket(recvBuffer);

			curTotalRecvSize -= packetSize;
			memcpy(recvBuffer, recvBuffer + packetSize, curTotalRecvSize);                     
		}
	}
	return 0;
}

void TCPClient::ProcessPacket(char* _packet)
{
	char* tempPacket = _packet;						tempPacket += sizeof(u_short);
	u_short type = *(u_short*)tempPacket;			tempPacket += sizeof(u_short);

	switch ((ePacketType)type)
	{
	case ePacketType::S_Connect:
		m_packetHandler.S_Connect(tempPacket);
		break;
	}
}

unsigned int __stdcall TCPClient::ThreadFunc(void* _pArgs)
{
	TCPClient* pClient = static_cast<TCPClient*>(_pArgs);
	return pClient->ReceivePacket();
}
