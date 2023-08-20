#include "PacketHandler.h"
#include "../User.h"
#include "../Managers/UserManager.h"
#include "../Managers/SceneManager.h"
#include "../Managers/ResourceManager.h"
#include "../Scene/LoginScene.h"
#include "../Scene/LobbyScene.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIButton.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Managers/UIManager.h"

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
}

void PacketHandler::S_FailedLogin()
{
	UIPanel* pFailedLoginUI = dynamic_cast<UIPanel*>(UIManager::GetInst()->FindUI(L"FailedLogin"));
	pFailedLoginUI->SetActive(true);
	pFailedLoginUI->SetText(L"닉네임 중복입니다.");
	UIManager::GetInst()->AddPopupUI(pFailedLoginUI);
}

void PacketHandler::S_OKLogin()
{
	SceneManager::GetInst()->ChangeScene(new LobbyScene);
}
