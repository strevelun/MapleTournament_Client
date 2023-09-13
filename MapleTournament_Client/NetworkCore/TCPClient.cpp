#include "NetworkManager.h"
#include "TCPClient.h"

#include <iostream>
#include <thread>
#include <string>

std::map<ePacketType, void(*)(char*)> TCPClient::m_mapPacketHandlerCallback = {
	{ ePacketType::S_Exit, PacketHandler::S_Exit },
	{ ePacketType::S_OKLogin, PacketHandler::S_OKLogin },
	{ ePacketType::S_FailedLogin, PacketHandler::S_FailedLogin },
	{ ePacketType::S_CreateRoom, PacketHandler::S_CreateRoom },
	{ ePacketType::S_SendSessions, PacketHandler::S_SendSessions },
	{ ePacketType::S_SendRooms, PacketHandler::S_SendRooms },
	{ ePacketType::S_EnterOtherUser, PacketHandler::S_EnterOtherUser },
	{ ePacketType::S_Chat, PacketHandler::S_Chat },
	{ ePacketType::S_JoinRoom, PacketHandler::S_JoinRoom },
};

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
	int							recvSize, totalSize = 0;
	u_short						packetSize;
	char						recvBuffer[255];
	int bufferSize = sizeof(recvBuffer);

	bool isRunning = true;

	while (isRunning)
	{
		recvSize = NetworkManager::GetInst()->Receive(recvBuffer + totalSize, bufferSize - totalSize);
		if (recvSize == -1) {
			int error = WSAGetLastError();
			std::cout << ("Receive error: " + std::to_string(error)).c_str();
			// TODO : MEssagebox
			break;
		}

		totalSize += recvSize;

		while (1)
		{
			packetSize = *(u_short*)recvBuffer;
			if ( packetSize > totalSize) break;

			ProcessPacket(recvBuffer);

			totalSize -= packetSize;
			memcpy(recvBuffer, recvBuffer + packetSize, totalSize);                     
		}
	}
	return 0;
}

void TCPClient::ProcessPacket(char* _packet)
{
	char* tempPacket = _packet;						tempPacket += sizeof(u_short);
	u_short type = *(u_short*)tempPacket;			tempPacket += sizeof(u_short);

	m_mapPacketHandlerCallback[(ePacketType)type](tempPacket);
}

unsigned int __stdcall TCPClient::ThreadFunc(void* _pArgs)
{
	TCPClient* pClient = static_cast<TCPClient*>(_pArgs);
	return pClient->ReceivePacket();
}
