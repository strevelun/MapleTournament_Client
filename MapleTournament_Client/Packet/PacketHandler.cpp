#include "PacketHandler.h"
#include "../User.h"
#include "../Managers/UserManager.h"

typedef unsigned short ushort;

PacketHandler::PacketHandler()
{
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::S_Connect(char* _packet)
{
	UserManager* pUserManager = UserManager::GetInst();

	ushort id = *(ushort*)_packet;

	User* pUser = new User(id);
	pUserManager->AddUser(id, pUser);
	pUserManager->SetMyUser(pUser);

	//wchar_t msg[255];
	//wsprintfW(msg, L"id:%d 연결완료", id);
	//MessageBox(nullptr, msg, L"S_Connect패킷", MB_OK);
}
