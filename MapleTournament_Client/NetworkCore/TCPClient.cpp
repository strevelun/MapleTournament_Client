#include "NetworkManager.h"
#include "TCPClient.h"
#include "../Debug.h"

#include <iostream>
#include <thread>
#include <string>

std::map<ePacketType, void(*)(char*)> TCPClient::m_mapPacketHandlerCallback = {
	{ ePacketType::S_OKLogin, PacketHandler::S_OKLogin },
	{ ePacketType::S_FailedLogin, PacketHandler::S_FailedLogin },
	{ ePacketType::S_CreateRoom, PacketHandler::S_CreateRoom },
	{ ePacketType::S_Chat, PacketHandler::S_Chat },
	{ ePacketType::S_JoinRoom, PacketHandler::S_JoinRoom },
	{ ePacketType::S_NotifyJoinedUser, PacketHandler::S_NotifyJoinedUser },
	{ ePacketType::S_CheckRoomReadyOK, PacketHandler::S_CheckRoomReadyOK },
	{ ePacketType::S_CheckRoomReadyFail, PacketHandler::S_CheckRoomReadyFail },
	{ ePacketType::S_EnterLobby, PacketHandler::S_EnterLobby },
	{ ePacketType::S_UpdateRoomMemberLeave, PacketHandler::S_UpdateRoomMemberLeave },
	{ ePacketType::S_UpdateUserState, PacketHandler::S_UpdateUserState },
	{ ePacketType::S_UpdateUserType, PacketHandler::S_UpdateUserType },
	{ ePacketType::S_UpdateWaitingRoomBtn, PacketHandler::S_UpdateWaitingRoomBtn },
	{ ePacketType::S_InGameReady, PacketHandler::S_InGameReady },
	{ ePacketType::S_UpdateUserListPage, PacketHandler::S_UpdateUserListPage },
	{ ePacketType::S_UpdateRoomListPage, PacketHandler::S_UpdateRoomListPage },
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

	//unsigned int threadID;
	//m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, 0, &threadID);
	//if (m_hThread == NULL)
	//{
	//	Debug::Log("Failed BeginThreadEx");
	//	return false;
	//}
	return true;
}

void TCPClient::ReceivePacket()
{
	int	recvSize = 0;
	int bufferSize = sizeof(m_recvBuffer);

	recvSize = NetworkManager::GetInst()->Receive(m_recvBuffer + m_totalSize, bufferSize - m_totalSize);
	if (recvSize < 0)
	{
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK)
		{
			Debug::Log("Receive error: " + std::to_string(error));
		}
		return;
	}

	m_totalSize += recvSize;

	while (m_totalSize >= sizeof(u_short))
	{
		m_packetSize = *(u_short*)m_recvBuffer;
		if (m_packetSize > m_totalSize) break;

		ProcessPacket(m_recvBuffer);

		m_totalSize -= m_packetSize;
		memcpy(m_recvBuffer, m_recvBuffer + m_packetSize, m_totalSize);
	}

	return;
}

void TCPClient::ProcessPacket(char* _packet)
{
	char* tempPacket = _packet;						tempPacket += sizeof(u_short);
	u_short type = *(u_short*)tempPacket;			tempPacket += sizeof(u_short);

	m_mapPacketHandlerCallback[(ePacketType)type](tempPacket);
}
/*
unsigned int __stdcall TCPClient::ThreadFunc(void* _pArgs)
{
	TCPClient* pClient = static_cast<TCPClient*>(_pArgs);
	return pClient->ReceivePacket();
}
*/