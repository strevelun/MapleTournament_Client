#include "PacketHandler.h"
#include "../NetworkCore/NetworkManager.h"
#include "../Managers/SceneManager.h"
#include "../Managers/ResourceManager.h"
#include "../Scene/LoginScene.h"
#include "../Scene/LobbyScene.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIButton.h"
#include "../Obj/UI/UIList.h"
#include "../Obj/UI/UIText.h"
#include "../Obj/UI/Mouse.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Managers/UIManager.h"
#include "../Managers/ObjectManager.h"
#include "../Managers/InputManager.h"
#include "../Obj/MyPlayer.h"

typedef unsigned short ushort;

void PacketHandler::S_Exit(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (!pUI) return;
	UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
	pUI = pUserListPanel->FindChildUI(L"UserList");
	UIList* pUserList = static_cast<UIList*>(pUI);
	pUserList->RemoveItem((wchar_t*)_packet);
}

void PacketHandler::S_OKLogin(char* _packet)
{
	MyPlayer* pMyPlayer = new MyPlayer((wchar_t*)_packet);
	ObjectManager::GetInst()->SetMyPlayer(pMyPlayer);
	ObjectManager::GetInst()->AddObj(pMyPlayer);

	SceneManager::GetInst()->ChangeScene(new LobbyScene);


}

void PacketHandler::S_FailedLogin(char* _packet) // 누가 이 닉네임으로 로그인 중 or 회원가입 안된 닉네임
{
	UI* pUI = UIManager::GetInst()->FindUI(L"FailedLogin");
	if (!pUI) return;
	UIPanel* pFailedLoginUI = static_cast<UIPanel*>(pUI);
	pFailedLoginUI->SetActive(true);
	UIText* pText = new UIText(pFailedLoginUI, L"누군가 이 닉네임으로 로그인 중",
		wcslen(L"누군가 이 닉네임으로 로그인 중") * 25, 25, pFailedLoginUI->GetWidth() / 2, 100, 0.5f, 0.5f);
	pFailedLoginUI->AddChildUI(pText);
	UIManager::GetInst()->AddPopupUI(pFailedLoginUI);
}

void PacketHandler::S_CreateRoom(char* _packet)
{
	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	pMouse->SetActive(true);

	// UserList, Chat, Roomlist 전부 삭제
	UI* pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (!pUI) return;
	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pUI = pPanel->FindChildUI(L"ChatList");
	UIList* pList = static_cast<UIList*>(pUI);
	pList->RemoveAllItems();

	pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (!pUI) return;
	UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
	pUI = pUserListPanel->FindChildUI(L"UserList");
	pList = static_cast<UIList*>(pUI);
	pList->RemoveAllItems();

	pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (!pUI) return;
	UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
	pUI = pRoomListPanel->FindChildUI(L"RoomList");
	pList = static_cast<UIList*>(pUI);
	pList->RemoveAllItems();

	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	pLobbyScene->HideLobbyUI();
	pLobbyScene->ShowWaitingRoomUI();

	MyPlayer* pPlayer = ObjectManager::GetInst()->GetMyPlayer();
	const std::wstring& strNickname = pPlayer->GetNickname();
	
	pUI = UIManager::GetInst()->FindUI(L"UserSlot1");
	if (!pUI) return;
	UIPanel* pSlot = static_cast<UIPanel*>(pUI);
	pUI = pSlot->FindChildUI(L"Nickname");
	if (!pUI) return;
	UIText* pTextNickname = static_cast<UIText*>(pUI);
	pTextNickname->ReassignText(strNickname, eTextSize::Small);

	pUI = pSlot->FindChildUI(L"State");
	if (!pUI) return;
	UIPanel* pTextState = static_cast<UIPanel*>(pUI);
	Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_owner.png");
	if (!pBitmap) return;
	pTextState->SetBitmap(pBitmap);
	pSlot->SetActive(true);
}

// 1 : Ready, 2 : InGame
void PacketHandler::S_NotifyCreateRoom(char* _packet)
{
	unsigned int roomId = *(unsigned int*)_packet;			_packet += sizeof(unsigned int);
	std::wstring strRoomId = std::to_wstring(roomId);
	std::wstring strRoomState(L"Ready");
	wchar_t* roomTitle = (wchar_t*)_packet;					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
	wchar_t* roomOwner = (wchar_t*)_packet;					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (!pUI) return;
	UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
	pUI = pRoomListPanel->FindChildUI(L"RoomList");
	UIList* pRoomList = static_cast<UIList*>(pUI);
	UIPanel* pPanel = new UIPanel(pRoomList, 660, 15);
	UIButton* pBtn = new UIButton(pPanel, 660, 15);
	pBtn->SetName(strRoomId);
	pBtn->SetClickable(true);
	pBtn->SetCallback([roomId]()
		{
			char buffer[255];
			ushort count = sizeof(ushort);
			*(ushort*)(buffer + count) = (ushort)ePacketType::C_JoinRoom;				count += sizeof(ushort);
			*(unsigned int*)(buffer + count) = roomId;									count += sizeof(unsigned int);
			*(ushort*)buffer = count;
			NetworkManager::GetInst()->Send(buffer);
			Mouse* pMouse = InputManager::GetInst()->GetMouse();
			pMouse->SetActive(false);
		});
	pPanel->AddChildUI(pBtn);
	UIText* pTextRoomId = new UIText(pPanel, strRoomId, strRoomId.length() * 15, 15, 0.f, 0.f, 0.f, 0.f, eTextSize::Small);
	pPanel->AddChildUI(pTextRoomId);
	UIText* pTextState = new UIText(pPanel, strRoomState, strRoomState.length() * 15, 15, 80.f, 0.f, 0.f, 0.f, eTextSize::Small);
	pPanel->AddChildUI(pTextState);
	UIText* pTextTitle = new UIText(pPanel, roomTitle, wcslen(roomTitle) * 15, 15, 140.f, 0.f, 0.f, 0.f, eTextSize::Small);
	pPanel->AddChildUI(pTextTitle);
	UIText* pTextOwner = new UIText(pPanel, roomOwner, wcslen(roomOwner) * 15, 15, 460.f, 0.f, 0.f, 0.f, eTextSize::Small);
	pPanel->AddChildUI(pTextOwner);
	UIText* pTextParticipants = new UIText(pPanel, L"1 / 4", wcslen(L"1 / 4") * 15, 15, 590.f, 0.f, 0.f, 0.f, eTextSize::Small);
	pPanel->AddChildUI(pTextParticipants);
	pRoomList->AddItem(pPanel);
}


// 처음 유저가 로비에 접속하면 현재 로그인 중인 세션 목록을 가져옴.
// 유저들 보낸다 
// 받은 패킷은 유저리스트 
//void PacketHandler::ReceiveUserList(char* _packet)
void PacketHandler::S_SendSessions(char* _packet)
{
	int size = *(ushort*)_packet;			_packet += sizeof(ushort);
	UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (!pUI) return;
	UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
	pUI = pUserListPanel->FindChildUI(L"UserList");
	UIList* pList = static_cast<UIList*>(pUI);

	for (int i = 0; i < size; i++)
	{				
		UIText* pText = new UIText(pList, (wchar_t*)_packet, wcslen((wchar_t*)_packet) * 25, 25, 10);
		pText->SetName((wchar_t*)_packet);
		_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		pList->AddItem(pText);
	}
}

void PacketHandler::S_SendRooms(char* _packet)
{
	u_short size = *(ushort*)_packet;						_packet += sizeof(ushort);
	for (int i = 0; i < size; i++)
	{
		unsigned int roomId = *(unsigned int*)_packet;			_packet += sizeof(unsigned int);
		ushort eRoomState = *(ushort*)_packet;					_packet += sizeof(ushort);
		wchar_t* roomTitle = (wchar_t*)_packet;					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		wchar_t* roomOwner = (wchar_t*)_packet;					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		ushort userCount = *(wchar_t*)_packet;					_packet += sizeof(ushort);
		std::wstring strRoomState(eRoomState == 1 ? L"Ready" : L"InGame");
		std::wstring strRoomId = std::to_wstring(roomId);
		std::wstring strUserCount(std::to_wstring(userCount) + L" / 4");

		UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
		if (!pUI) return;
		UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
		pUI = pRoomListPanel->FindChildUI(L"RoomList");
		UIList* pRoomList = static_cast<UIList*>(pUI);
		UIPanel* pPanel = new UIPanel(pRoomList, 660, 15);
		UIButton* pBtn = new UIButton(pPanel, 660, 15);
		pBtn->SetName(strRoomId);
		pBtn->SetClickable(true);
		pBtn->SetCallback([roomId]()
			{
				char buffer[255];
				ushort count = sizeof(ushort);
				*(ushort*)(buffer + count) = (ushort)ePacketType::C_JoinRoom;				count += sizeof(ushort);
				*(unsigned int*)(buffer + count) = roomId;									count += sizeof(unsigned int);
				*(ushort*)buffer = count;
				NetworkManager::GetInst()->Send(buffer);
				Mouse* pMouse = InputManager::GetInst()->GetMouse();
				pMouse->SetActive(false);
			});
		pPanel->AddChildUI(pBtn);
		UIText* pTextRoomId = new UIText(pPanel, strRoomId, strRoomId.length() * 15, 15, 0.f, 0.f, 0.f, 0.f, eTextSize::Small);
		pPanel->AddChildUI(pTextRoomId);
		UIText* pTextState = new UIText(pPanel, strRoomState, strRoomState.length() * 15, 15, 80.f, 0.f, 0.f, 0.f, eTextSize::Small);
		pPanel->AddChildUI(pTextState);
		UIText* pTextTitle = new UIText(pPanel, roomTitle, wcslen(roomTitle) * 15, 15, 140.f, 0.f, 0.f, 0.f, eTextSize::Small);
		pPanel->AddChildUI(pTextTitle);
		UIText* pTextMaker = new UIText(pPanel, roomOwner, wcslen(roomOwner) * 15, 15, 460.f, 0.f, 0.f, 0.f, eTextSize::Small);
		pPanel->AddChildUI(pTextMaker);
		UIText* pTextParticipants = new UIText(pPanel, strUserCount, strUserCount.length() * 15, 15, 590.f, 0.f, 0.f, 0.f, eTextSize::Small);
		pPanel->AddChildUI(pTextParticipants);
		pRoomList->AddItem(pPanel);
	}
}

void PacketHandler::S_EnterOtherUser(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (!pUI) return;
	UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
	pUI = pUserListPanel->FindChildUI(L"UserList");
	UIList* pList = static_cast<UIList*>(pUI);
	UIText* pText = new UIText(pList, (wchar_t*)_packet, wcslen((wchar_t*)_packet) * 25, 25, 10);
	pText->SetName((wchar_t*)_packet);
	pList->AddItem(pText);
}

void PacketHandler::S_Chat(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (!pUI) return;
	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pUI = pPanel->FindChildUI(L"ChatList");
	UIList* pList = static_cast<UIList*>(pUI);
	std::wstring chat((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
	std::wstring nickname((wchar_t*)_packet);
	std::wstring message = nickname + L" : " + chat;

	UIText* pText = new UIText(pList, message, message.length() * 15, 15, 10, 50, 0.f, 0.f, eTextSize::Small);
	pList->AddItem(pText);
}

void PacketHandler::S_JoinRoom(char* _packet)
{
	UI* pUI;
	int userCount = *(ushort*)_packet;				_packet += sizeof(ushort);
	for (int i = 0; i < userCount; i++)
	{
		ushort slotLocation = *(ushort*)_packet;				_packet += sizeof(ushort);
		eMemberType eType = (eMemberType) * (ushort*)_packet;				_packet += sizeof(ushort);
		std::wstring nickname((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

		std::wstring uiName = L"UserSlot" + std::to_wstring(slotLocation);
		pUI = UIManager::GetInst()->FindUI(uiName);
		if (!pUI) return;
		UIPanel* pSlot = static_cast<UIPanel*>(pUI);
		pUI = pSlot->FindChildUI(L"Nickname");
		if (!pUI) return;
		UIText* pTextNickname = static_cast<UIText*>(pUI);
		pTextNickname->ReassignText(nickname, eTextSize::Small);
		Bitmap* pBitmap;
		if (eType == eMemberType::Owner)
		{
			pUI = pSlot->FindChildUI(L"State");
			if (!pUI) return;
			UIPanel* pTextState = static_cast<UIPanel*>(pUI);
			pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_owner.png");
			if (!pBitmap) return;
			pTextState->SetBitmap(pBitmap);
		}
		pSlot->SetActive(true);
	}

	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	pMouse->SetActive(true);

	// UserList, Chat, Roomlist 전부 삭제
	pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (!pUI) return;
	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pUI = pPanel->FindChildUI(L"ChatList");
	UIList* pList = static_cast<UIList*>(pUI);
	pList->RemoveAllItems();

	pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (!pUI) return;
	UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
	pUI = pUserListPanel->FindChildUI(L"UserList");
	pList = static_cast<UIList*>(pUI);
	pList->RemoveAllItems();

	pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (!pUI) return;
	UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
	pUI = pRoomListPanel->FindChildUI(L"RoomList");
	pList = static_cast<UIList*>(pUI);
	pList->RemoveAllItems();

	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	pLobbyScene->HideLobbyUI();
	pLobbyScene->ShowWaitingRoomUI();
}

void PacketHandler::S_JoinRoomFail(char* _packet)
{
	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	pMouse->SetActive(true);

	UI* pUI = UIManager::GetInst()->FindUI(L"JoinRoomFail");
	if (!pUI) return;
	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	UIManager::GetInst()->AddPopupUI(pPanel);
	pUI->SetActive(true);
}

void PacketHandler::S_NotifyJoinedUser(char* _packet)
{
	ushort slotLocation = *(ushort*)_packet;				_packet += sizeof(ushort);
	eMemberType eType = (eMemberType) * (ushort*)_packet;				_packet += sizeof(ushort);
	std::wstring nickname((wchar_t*)_packet);

	std::wstring uiName = L"UserSlot" + std::to_wstring(slotLocation);
	UI* pUI = UIManager::GetInst()->FindUI(uiName);
	if (!pUI) return;
	UIPanel* pSlot = static_cast<UIPanel*>(pUI);
	pUI = pSlot->FindChildUI(L"Nickname");
	if (!pUI) return;
	UIText* pTextNickname = static_cast<UIText*>(pUI);
	pTextNickname->ReassignText(nickname, eTextSize::Small);
	pSlot->SetActive(true);
}

