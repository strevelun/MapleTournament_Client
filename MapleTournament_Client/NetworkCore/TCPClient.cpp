#include "NetworkManager.h"
#include "TCPClient.h"
#include "../Packet/Packet.h"
#include "../Packet/EnterPacket.h"

#include <thread>
#include <string>

TCPClient::TCPClient()
	: m_bIsRunning(true), m_hThread(nullptr)
{
}

TCPClient::~TCPClient()
{
    CloseHandle(m_hThread);

	NetworkManager::DestroyInst();
}

bool TCPClient::Init()
{
	if (!NetworkManager::GetInst()->Init("192.168.219.167", 30001)) return false;

	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, 0, &threadID);
	if (m_hThread == NULL)
	{
		OutputDebugStringW(L"Failed BeginThreadEx");
		return false;
	}
	return true;
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
		recvSize = NetworkManager::GetInst()->Receive(recvBuffer + curTotalRecvSize, bufferSize - curTotalRecvSize);
		if (recvSize == -1) {
			int error = WSAGetLastError();
			OutputDebugStringA(("Receive error: " + std::to_string(error)).c_str());
		}

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
