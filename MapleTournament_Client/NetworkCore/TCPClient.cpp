#include "NetworkManager.h"
#include "TCPClient.h"
#include "../Debug.h"

#include <iostream>
#include <string>

std::map<ePacketType, void(*)(char*)> TCPClient::m_mapPacketHandlerCallback = {
	{ ePacketType::S_OKLogin, PacketHandler::S_OKLogin },
	{ ePacketType::S_FailedLogin, PacketHandler::S_FailedLogin },
	{ ePacketType::S_CreateRoom, PacketHandler::S_CreateRoom },
	{ ePacketType::S_Chat, PacketHandler::S_Chat },
	{ ePacketType::S_JoinRoom, PacketHandler::S_JoinRoom },
	{ ePacketType::S_JoinRoomFail, PacketHandler::S_JoinRoomFail },
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
	{ ePacketType::S_UpdateUserSlot, PacketHandler::S_UpdateUserSlot },
	{ ePacketType::S_Skill, PacketHandler::S_Skill },
	{ ePacketType::S_UpdateTurn, PacketHandler::S_UpdateTurn },
	{ ePacketType::S_UpdateDashboard, PacketHandler::S_UpdateDashboard },
	{ ePacketType::S_GameOver, PacketHandler::S_GameOver },
	{ ePacketType::S_GameOverSceneChange, PacketHandler::S_GameOverSceneChange },
	{ ePacketType::S_UpdateIngameUserLeave, PacketHandler::S_UpdateIngameUserLeave },
	{ ePacketType::S_Standby, PacketHandler::S_Standby },
	{ ePacketType::S_UpdateProfile, PacketHandler::S_UpdateProfile },
	{ ePacketType::S_CheckHit, PacketHandler::S_CheckHit },
	{ ePacketType::S_UpdateHeal, PacketHandler::S_UpdateHeal },
	{ ePacketType::S_CreatePortal, PacketHandler::S_CreatePortal },
	{ ePacketType::S_Teleport, PacketHandler::S_Teleport },
	{ ePacketType::S_ExitInGame, PacketHandler::S_ExitInGame },
};

TCPClient::TCPClient()
	: m_bIsRunning(true)
{
}

TCPClient::~TCPClient()
{
	NetworkManager::DestroyInst();
}

bool TCPClient::Init(const char* _serverIP, int _serverPort) // ip주소 여기로 옮기기
{
	if (!NetworkManager::GetInst()->Init(_serverIP, _serverPort)) return false;

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