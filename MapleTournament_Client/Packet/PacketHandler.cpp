#include "../NetworkCore/NetworkManager.h"
#include "PacketHandler.h"
#include "../Managers/SceneManager.h"
#include "../Managers/ResourceManager.h"
#include "../Scene/LoginScene.h"
#include "../Scene/LobbyScene.h"
#include "../Scene/InGameScene.h"
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
#include "../Animation/AnimationClip.h"
#include "../Animation/Animator.h"
#include "../Obj/MyPlayer.h"
#include "../Debug.h"
#include "../Constants.h"
//#include "../InstructionQueue.h"

typedef unsigned short ushort;

void PacketHandler::S_Exit(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (pUI)
	{
		UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
		pUI = pUserListPanel->FindChildUI(L"UserList");
		UIList* pUserList = static_cast<UIList*>(pUI);
		std::wstring user((wchar_t*)_packet);
		pUserList->RemoveItem(user);
	}

	Debug::Log("PacketHandler::S_Exit");
}

void PacketHandler::S_OKLogin(char* _packet)
{
	MyPlayer* pMyPlayer = new MyPlayer((wchar_t*)_packet);
	ObjectManager::GetInst()->SetMyPlayer(pMyPlayer);
	ObjectManager::GetInst()->AddObj(pMyPlayer);

	SceneManager::GetInst()->ChangeScene(new LobbyScene);

	Debug::Log("PacketHandler::S_OKLogin");
}

void PacketHandler::S_FailedLogin(char* _packet) // 누가 이 닉네임으로 로그인 중 or 회원가입 안된 닉네임
{
	UI* pUI = UIManager::GetInst()->FindUI(L"AlreadyLogin");
	if (pUI)
	{
		UIPanel* pFailedLoginUI = static_cast<UIPanel*>(pUI);
		pFailedLoginUI->SetActive(true);
		UIManager::GetInst()->AddPopupUI(pFailedLoginUI);
	}

	Debug::Log("PacketHandler::S_FailedLogin");
}

void PacketHandler::S_CreateRoom(char* _packet)
{
	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	pMouse->SetActive(true);

	UI* pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"ChatList");
		UIList* pList = static_cast<UIList*>(pUI);
		pList->RemoveAllItems();
	}

	pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (pUI)
	{
		UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
		pUI = pUserListPanel->FindChildUI(L"UserList");
		UIList* pList = static_cast<UIList*>(pUI);
		pList->RemoveAllItems();
	}

	pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (pUI)
	{
		UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
		pUI = pRoomListPanel->FindChildUI(L"RoomList");
		UIList* pList = static_cast<UIList*>(pUI);
		pList->RemoveAllItems();
	}

	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	pLobbyScene->HideLobbyUI();
	pLobbyScene->ShowWaitingRoomUI();

	MyPlayer* pPlayer = ObjectManager::GetInst()->GetMyPlayer();
	const std::wstring& strNickname = pPlayer->GetNickname();
	
	pUI = UIManager::GetInst()->FindUI(L"UserSlot0");
	if (pUI)
	{
		UIPanel* pSlot = static_cast<UIPanel*>(pUI);
		pUI = pSlot->FindChildUI(L"Nickname");
		if (pUI)
		{
			UIText* pTextNickname = static_cast<UIText*>(pUI);
			pTextNickname->ReassignText(strNickname, 20.f);
		}
		pUI = pSlot->FindChildUI(L"State");
		if (pUI)
		{
			UIPanel* pTextState = static_cast<UIPanel*>(pUI);
			Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_owner.png");
			if (pBitmap)
				pTextState->SetBitmap(pBitmap);
		}
		pSlot->SetActive(true);
	}

	pUI = UIManager::GetInst()->FindUI(L"StartGame");
	if (pUI)
	{
		UIButton* pBtn = static_cast<UIButton*>(pUI);
		pBtn->SetCallback(&LobbyScene::GameStartCallback, pLobbyScene);
	}

	Debug::Log("PacketHandler::S_CreateRoom");
}

// 1 : Ready, 2 : InGame
void PacketHandler::S_NotifyCreateRoom(char* _packet)
{
	unsigned int roomId = *(unsigned int*)_packet;			_packet += sizeof(unsigned int);
	std::wstring strRoomId = std::to_wstring(roomId);
	std::wstring strRoomState(L"Ready");
	std::wstring roomTitle = (wchar_t*)_packet;					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
	std::wstring roomOwner = (wchar_t*)_packet;					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (pUI)
	{
		UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
		pUI = pRoomListPanel->FindChildUI(L"RoomList");
		UIList* pRoomList = static_cast<UIList*>(pUI);
		UIPanel* pPanel = new UIPanel(pRoomList, 660, 15);
		pPanel->SetName(strRoomId);
		UIButton* pBtn = new UIButton(pPanel, 660, 15);
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
		UIText* pTextRoomId = new UIText(pPanel, strRoomId, 20.f, 0.f, 0.f, 0.f, 0.f);
		pPanel->AddChildUI(pTextRoomId);
		UIText* pTextState = new UIText(pPanel, strRoomState, 20.f, 80.f, 0.f, 0.f, 0.f);
		pTextState->SetName(L"State");
		pPanel->AddChildUI(pTextState);
		UIText* pTextTitle = new UIText(pPanel, roomTitle, 20.f, 140.f, 0.f, 0.f, 0.f);
		pPanel->AddChildUI(pTextTitle);
		UIText* pTextOwner = new UIText(pPanel, roomOwner, 20.f, 460.f, 0.f, 0.f, 0.f);
		pTextOwner->SetName(L"Owner");
		pPanel->AddChildUI(pTextOwner);
		UIText* pTextParticipants = new UIText(pPanel, L"1 / 4", 20.f, 590.f, 0.f, 0.f, 0.f);
		pTextParticipants->SetName(L"Count");
		pPanel->AddChildUI(pTextParticipants);
		pRoomList->AddItem(pPanel);
	}

	Debug::Log("PacketHandler::S_NotifyCreateRoom");
}


// 처음 유저가 로비에 접속하면 현재 로그인 중인 세션 목록을 가져옴.
// 유저들 보낸다 
// 받은 패킷은 유저리스트 
//void PacketHandler::ReceiveUserList(char* _packet)
void PacketHandler::S_SendSessions(char* _packet)
{
	int size = *(ushort*)_packet;			_packet += sizeof(ushort);

	UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (pUI)
	{
		UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
		pUI = pUserListPanel->FindChildUI(L"UserList");
		UIList* pList = static_cast<UIList*>(pUI);

		for (int i = 0; i < size; i++)
		{
			UIText* pText = new UIText(pList, (wchar_t*)_packet, 20.f, 10);
			pText->SetName((wchar_t*)_packet);
			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
			pList->AddItem(pText);
		}
	}
	
	Debug::Log("PacketHandler::S_SendSessions");
}

// S_RoomList
void PacketHandler::S_SendRooms(char* _packet)
{
	u_short size = *(ushort*)_packet;							_packet += sizeof(ushort);

	for (int i = 0; i < size; i++)
	{
		unsigned int roomId = *(unsigned int*)_packet;			_packet += sizeof(unsigned int);
		ushort eRoomState = *(ushort*)_packet;					_packet += sizeof(ushort);
		std::wstring roomTitle = (wchar_t*)_packet;				_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		std::wstring  roomOwner = (wchar_t*)_packet;			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		ushort userCount = *(wchar_t*)_packet;					_packet += sizeof(ushort);

		// TODO 로컬변수 만들지말고 바로 전달
		std::wstring strRoomState(eRoomState == 1 ? L"Ready" : L"InGame");
		std::wstring strRoomId = std::to_wstring(roomId);
		std::wstring strUserCount(std::to_wstring(userCount) + L" / 4");

		UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
		if (pUI)
		{
			UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
			pUI = pRoomListPanel->FindChildUI(L"RoomList");
			UIList* pRoomList = static_cast<UIList*>(pUI);
			UIPanel* pPanel = new UIPanel(pRoomList, 660, 15);
			pPanel->SetName(strRoomId);
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
			UIText* pTextRoomId = new UIText(pPanel, strRoomId, 20.f, 0.f, 0.f, 0.f, 0.f);
			pPanel->AddChildUI(pTextRoomId);
			UIText* pTextState = new UIText(pPanel, strRoomState, 20.f, 80.f, 0.f, 0.f, 0.f);
			pTextState->SetName(L"State");
			pPanel->AddChildUI(pTextState);
			UIText* pTextTitle = new UIText(pPanel, roomTitle, 20.f, 140.f, 0.f, 0.f, 0.f);
			pPanel->AddChildUI(pTextTitle);
			UIText* pTextOwner = new UIText(pPanel, roomOwner, 20.f, 460.f, 0.f, 0.f, 0.f);
			pTextOwner->SetName(L"Owner");
			pPanel->AddChildUI(pTextOwner);
			UIText* pTextParticipants = new UIText(pPanel, strUserCount, 20.f, 590.f, 0.f, 0.f, 0.f);
			pTextParticipants->SetName(L"Count");
			pPanel->AddChildUI(pTextParticipants);
			pRoomList->AddItem(pPanel);
		}
	}
	Debug::Log("PacketHandler::S_SendRooms");
}

// TODO : EnterLobbyOtherUser
void PacketHandler::S_EnterOtherUser(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (pUI)
	{
		UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
		pUI = pUserListPanel->FindChildUI(L"UserList");
		UIList* pList = static_cast<UIList*>(pUI);
		UIText* pText = new UIText(pList, (wchar_t*)_packet, 20.f, 10);
		pText->SetName((wchar_t*)_packet);
		pList->AddItem(pText);
	}

	Debug::Log("PacketHandler::S_EnterOtherUser");
}

void PacketHandler::S_Chat(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"ChatList");
		UIList* pList = static_cast<UIList*>(pUI);
		std::wstring chat((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		std::wstring nickname((wchar_t*)_packet);
		std::wstring message = nickname + L" : " + chat;

		UIText* pText = new UIText(pList, message, 20.f, 10, 50, 0.f, 0.f);
		pList->AddItem(pText);
	}

	Debug::Log("PacketHandler::S_Chat");
}

void PacketHandler::S_JoinRoom(char* _packet)
{
	UI* pUI;
	int userCount = *(ushort*)_packet;				_packet += sizeof(ushort);

	for (int i = 0; i < userCount; i++)
	{
		char slotLocation = *(char*)_packet;				_packet += sizeof(char);
		eMemberType eType = (eMemberType) * (ushort*)_packet;				_packet += sizeof(ushort);
		std::wstring nickname((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

		pUI = UIManager::GetInst()->FindUI(L"UserSlot" + std::to_wstring(slotLocation));
		if (pUI)
		{
			UIPanel* pSlot = static_cast<UIPanel*>(pUI);
			pUI = pSlot->FindChildUI(L"Nickname");
			if (pUI)
			{
				UIText* pTextNickname = static_cast<UIText*>(pUI);
				pTextNickname->ReassignText(nickname, 20.f);
			}
			Bitmap* pBitmap = nullptr;
			pUI = pSlot->FindChildUI(L"State");
			if (pUI)
			{
				UIPanel* pTextState = static_cast<UIPanel*>(pUI);
				if (eType == eMemberType::Owner)
				{
					pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_owner.png");
				}
				else if (eType == eMemberType::Member)
				{
					pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
				}
				if (pBitmap) 
					pTextState->SetBitmap(pBitmap);
			}
			pSlot->SetActive(true);
		}
	}

	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	pMouse->SetActive(true);

	UIList* pList;
	// UserList, Chat, Roomlist 전부 삭제
	pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"ChatList");
		if (pUI)
		{
			pList = static_cast<UIList*>(pUI);
			pList->RemoveAllItems();
			pUI->SetActive(true);
		}
	}

	pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (pUI)
	{
		pUI->SetActive(false);
		UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
		pUI = pUserListPanel->FindChildUI(L"UserList");
		if (pUI)
		{
			pList = static_cast<UIList*>(pUI);
			pList->RemoveAllItems();
			pUI->SetActive(true);
		}
	}

	pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (pUI)
	{
		pUI->SetActive(false);
		UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
		pUI = pRoomListPanel->FindChildUI(L"RoomList");
		if (pUI)
		{
			pList = static_cast<UIList*>(pUI);
			pList->RemoveAllItems();
			pUI->SetActive(true);
		}
	}

	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	pLobbyScene->HideLobbyUI();
	pLobbyScene->ShowWaitingRoomUI();

	pUI = UIManager::GetInst()->FindUI(L"StartGame");
	if (pUI)
	{
		UIButton* pBtn = static_cast<UIButton*>(pUI);
		pBtn->SetCallback(&LobbyScene::GameReadyCallback, pLobbyScene);
		UIText* pText = pBtn->GetUIText();
		pText->ReassignText(L"준비", 20.f);
	}

	Debug::Log("PacketHandler::S_JoinRoom");
}

void PacketHandler::S_JoinRoomFail(char* _packet)
{
	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	pMouse->SetActive(true);

	UI* pUI = UIManager::GetInst()->FindUI(L"JoinRoomFail");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		UIManager::GetInst()->AddPopupUI(pPanel);
		pUI->SetActive(true);
	}

	Debug::Log("PacketHandler::S_JoinRoomFail");
}

void PacketHandler::S_NotifyJoinedUser(char* _packet)
{
	char slotLocation = *(char*)_packet;				_packet += sizeof(char);
	eMemberType eType = (eMemberType) * (ushort*)_packet;				_packet += sizeof(ushort);
	std::wstring nickname((wchar_t*)_packet);

	std::wstring uiName = L"UserSlot" + std::to_wstring(slotLocation);
	UI* pUI = UIManager::GetInst()->FindUI(uiName);
	if (pUI)
	{
		UIPanel* pSlot = static_cast<UIPanel*>(pUI);
		pUI = pSlot->FindChildUI(L"Nickname");
		if (pUI)
		{
			UIText* pTextNickname = static_cast<UIText*>(pUI);
			pTextNickname->ReassignText(nickname, 20.f);
		}
		pUI = pSlot->FindChildUI(L"State");
		if (pUI)
		{
			Bitmap* pBitmap = nullptr;
			UIPanel* pTextState = static_cast<UIPanel*>(pUI);
			if (eType == eMemberType::Owner)
			{
				pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_owner.png");
			}
			else if (eType == eMemberType::Member)
			{
				pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
			}
			if (pBitmap)
				pTextState->SetBitmap(pBitmap);
		}
		pSlot->SetActive(true);
	}

	Debug::Log("PacketHandler::S_NotifyJoinedUser");
}

void PacketHandler::S_CheckRoomReadyOK(char* _packet)
{
	SceneManager::GetInst()->ChangeScene(new InGameScene);

	Debug::Log("PacketHandler::S_CheckRoomReadyOK");
}

void PacketHandler::S_CheckRoomReadyFail(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"StartGameFail");
	if (pUI)
	{
		UIPanel* popup = static_cast<UIPanel*>(pUI);
		popup->SetActive(true);
		UIManager::GetInst()->AddPopupUI(popup);
	}

	Debug::Log("PacketHandler::S_CheckRoomReadyFail");
}

void PacketHandler::S_EnterLobby(char* _packet)
{
	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	pLobbyScene->HideWaitingRoomUI();
	pLobbyScene->ShowLobbyUI();

	Debug::Log("PacketHandler::S_EnterLobby");
}

void PacketHandler::S_LeaveRoom(char* _packet)
{
	unsigned int roomId = *(unsigned int*)_packet;			_packet += sizeof(unsigned int);
	std::wstring strRoomId = std::to_wstring(roomId);

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (pUI)
	{
		UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
		pUI = pRoomListPanel->FindChildUI(L"RoomList");
		UIList* pList = static_cast<UIList*>(pUI);
		pList->RemoveItem(strRoomId);
	}

	Debug::Log("PacketHandler::S_LeaveRoom");
}

void PacketHandler::S_UpdateRoomMemberLeave(char* _packet)
{
	char leftSlot = *(char*)_packet;				_packet += sizeof(char);
	char newOwnerSlot = *(char*)_packet;				_packet += sizeof(char);

	UI* pUI = UIManager::GetInst()->FindUI(L"UserSlot" + std::to_wstring(leftSlot));
	if (!pUI) return;
	pUI->SetActive(false);

	pUI = UIManager::GetInst()->FindUI(L"UserSlot" + std::to_wstring(newOwnerSlot));
	if (pUI)
	{
		UIPanel* pSlot = static_cast<UIPanel*>(pUI);
		Bitmap* pBitmap;
		pUI = pSlot->FindChildUI(L"State");
		if (pUI)
		{
			UIPanel* pTextState = static_cast<UIPanel*>(pUI);
			pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_owner.png");
			if (pBitmap)
				pTextState->SetBitmap(pBitmap);
		}
	}

	Debug::Log("PacketHandler::S_UpdateRoomMemberLeave");
}

// 갱신해 room list 
void PacketHandler::S_UpdateLobbyRoomList(char* _packet)
{
	unsigned int roomId = *(unsigned int*)_packet;				_packet += sizeof(unsigned int);
	std::wstring nickname((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
	char roomMemberCount = *(char*)_packet;

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (!pUI) return;
	UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
	pUI = pRoomListPanel->FindChildUI(L"RoomList");
	if (!pUI) return;
	UIList* pRoomList = static_cast<UIList*>(pUI);
	pUI = pRoomList->FindItem(std::to_wstring(roomId));
	if (!pUI) return;
	UIPanel* uiItem = static_cast<UIPanel*>(pUI);
	pUI = uiItem->FindChildUI(L"Owner");
	if (!pUI) return;
	UIText* pOwner = static_cast<UIText*>(pUI);
	pOwner->ReassignText(nickname, 20.f);
	pUI = uiItem->FindChildUI(L"Count");
	if (!pUI) return;
	UIText* pCount = static_cast<UIText*>(pUI);
	pCount->ReassignText(std::to_wstring(roomMemberCount) + L" / 4", 20.f);

	Debug::Log("PacketHandler::S_UpdateLobbyRoomList");
}

void PacketHandler::S_UpdateLobbyRoomMemberCount(char* _packet)
{
	unsigned int roomId = *(unsigned int*)_packet;				_packet += sizeof(unsigned int);
	char roomMemberCount = *(char*)_packet;

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (!pUI) return;
	UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
	pUI = pRoomListPanel->FindChildUI(L"RoomList");
	if (!pUI) return;
	UIList* pRoomList = static_cast<UIList*>(pUI);
	pUI = pRoomList->FindItem(std::to_wstring(roomId));
	if (!pUI) return;
	UIPanel* uiItem = static_cast<UIPanel*>(pUI);
	pUI = uiItem->FindChildUI(L"Count");
	if (!pUI) return;
	UIText* pCount = static_cast<UIText*>(pUI);
	pCount->ReassignText(std::to_wstring(roomMemberCount) + L" / 4", 20.f);

	Debug::Log("PacketHandler::S_UpdateLobbyRoomMemberCount");
}

void PacketHandler::S_UpdateUserState(char* _packet)
{
	eMemberState state = (eMemberState)*(char*)_packet;				_packet += sizeof(char);
	char slot = *(char*)_packet;									_packet += sizeof(char);

	UI* pUI = UIManager::GetInst()->FindUI(L"UserSlot" + std::to_wstring(slot));
	if (!pUI) return;
	UIPanel* pSlot = static_cast<UIPanel*>(pUI);
	pUI = pSlot->FindChildUI(L"State");
	if (!pUI) return;
	UIPanel* pTextState = static_cast<UIPanel*>(pUI);
	Bitmap* pBitmap = nullptr;
	if(state == eMemberState::Wait)
		pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
	else if(state == eMemberState::Ready)
		pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_ready.png");
	if (pBitmap)
		pTextState->SetBitmap(pBitmap);

	Debug::Log("PacketHandler::S_UpdateUserState");
}

void PacketHandler::S_UpdateUserType(char* _packet)
{
	eMemberType type = (eMemberType)*(char*)_packet;

	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	if (type == eMemberType::Owner)
	{
		UI* pUI = UIManager::GetInst()->FindUI(L"StartGame");
		if (!pUI) return;
		UIButton* pBtn = static_cast<UIButton*>(pUI);
		pBtn->SetCallback(&LobbyScene::GameStartCallback, pLobbyScene);
		UIText* pText = pBtn->GetUIText();
		pText->ReassignText(L"게임 시작", 20.f);
	}

	Debug::Log("PacketHandler::S_UpdateUserType");
}

void PacketHandler::S_UpdateWaitingRoomBtn(char* _packet)
{
	eMemberState state = (eMemberState)*(char*)_packet;
	UI* pUI = UIManager::GetInst()->FindUI(L"StartGame");
	if (!pUI) return;
	UIButton* pBtn = static_cast<UIButton*>(pUI);
	UIText* pText = pBtn->GetUIText();
	if (state == eMemberState::Wait)
	{
		pText->ReassignText(L"준비", 20.f);
	}
	else if (state == eMemberState::Ready)
	{
		pText->ReassignText(L"대기", 20.f);
	}

	Debug::Log("PacketHandler::S_UpdateWaitingRoomBtn");
}

void PacketHandler::S_InGameReady(char* _packet)
{
	int memberCount = *(char*)_packet;				_packet += sizeof(char);
	int mySlotNumber = *(char*)_packet;				_packet += sizeof(char); // 내 플레이어는 빨간색
	MyPlayer* myPlayer = nullptr;

	for (int i = 0; i < memberCount; i++)
	{
		int slot = *(char*)_packet;				_packet += sizeof(char);
		UI* pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
		if (!pUI) continue;
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"Nickname");
		if (pUI)
		{
			UIText* pText = static_cast<UIText*>(pUI);
			pText->ReassignText((wchar_t*)_packet, 20.f);
		}

		AnimationClip* pClip = ResourceManager::GetInst()->GetAnimClip(L"player" + std::to_wstring(slot));
		if (pClip)
		{
			pClip->SetLoop(false);
			pClip->SetPlayTime(1.5f);
			pClip->SetAnyState(true);
			pClip->SetFlip(true);
		}
		Animator* pAnimator = new Animator(pClip);

		myPlayer = new MyPlayer((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		myPlayer->SetPos(1030, 280);
		myPlayer->SetRatio(1.5f);
		myPlayer->SetAnimator(pAnimator);
		if(mySlotNumber == i)
			ObjectManager::GetInst()->SetMyPlayer(myPlayer);

		InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
		Layer* pLayer = pScene->FindLayer(L"Player");
		pLayer->AddObj(myPlayer);
	}

	/* 스킬 버튼 */
	std::list<UI*>* uiList = new std::list<UI*>();
	UIPanel* pPanel = new UIPanel(nullptr, 500, 100, ScreenWidth / 2, ScreenHeight, 0.5f, 1.0f);
	pPanel->SetName(L"SkillButtonPanel");
	Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_leftmove.png");
	UIButton* pButton = new UIButton(pPanel, 50, 50, 0, 0);
	pButton->SetBitmap(pBitmap);
	pButton->SetClickable(true);
	pButton->SetCallback([myPlayer] {
		myPlayer->UseSkill(eSkillType::LeftMove);
		});
	uiList->push_back(pButton);

	pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_leftdoublemove.png");
	pButton = new UIButton(pPanel, 50, 50, 50, 0);
	pButton->SetBitmap(pBitmap);
	pButton->SetClickable(true);
	pButton->SetCallback([myPlayer] {
		myPlayer->UseSkill(eSkillType::LeftDoubleMove);
		});
	uiList->push_back(pButton);

	pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_button_attackCloud.png");
	pButton = new UIButton(pPanel, 50, 50, 100, 0);
	pButton->SetBitmap(pBitmap);
	pButton->SetClickable(true);
	pButton->SetCallback([myPlayer] {
		myPlayer->UseSkill(eSkillType::AttackCloud);
		});
	uiList->push_back(pButton);

	std::list<UI*>::iterator iter = uiList->begin();
	std::list<UI*>::iterator iterEnd = uiList->end();
	for (;iter != iterEnd; iter++)
		pPanel->AddChildUI(*iter);
	UIManager::GetInst()->AddUI(pPanel);

	Debug::Log("PacketHandler::S_InGameReady");
}

