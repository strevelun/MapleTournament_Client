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
#include "../Obj/UI/UIScrollView.h"
#include "../Obj/UI/UIPage.h"
#include "../Obj/UI/Mouse.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Managers/UIManager.h"
#include "../Managers/ObjectManager.h"
#include "../Managers/InputManager.h"
#include "../Animation/AnimationClip.h"
#include "../Animation/Animator.h"
#include "../Obj/Player.h"
#include "../Obj/Skill.h"
#include "../Debug.h"
#include "../Constants.h"
//#include "../InstructionQueue.h"

typedef unsigned short ushort;

void PacketHandler::S_OKLogin(char* _packet)
{
	LobbyScene* pScene = new LobbyScene;
	SceneManager::GetInst()->ChangeScene(pScene);
	pScene->ChangeSceneUI(eSessionState::Lobby);

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

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomTitle");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"Text");
		if (pUI)
		{
			UIText* pText = static_cast<UIText*>(pUI);
			pText->ReassignText((wchar_t*)_packet);
		}
	}

	_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();

	pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"ChatList");
		UIScrollView* pScroll = static_cast<UIScrollView*>(pUI);
		pScroll->Clear();
	}

	// UserList는 WaitingRoom으로 갈때 active=false로 하고, 다시 lobby로 나올때 서버로부터 업데이트
	pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (pUI)
	{
		UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
		pUI = pUserListPanel->FindChildUI(L"UserList");
		UIPage* pPage = static_cast<UIPage*>(pUI);
		pPage->Reset();
	}

	pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (pUI)
	{
		UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
		pUI = pRoomListPanel->FindChildUI(L"RoomList");
		UIPage* pPage = static_cast<UIPage*>(pUI);
		pPage->Reset();
	}

	pLobbyScene->ChangeSceneUI(eSessionState::WaitingRoom);
	
	pUI = UIManager::GetInst()->FindUI(L"UserSlot0");
	if (pUI)
	{
		UIPanel* pSlot = static_cast<UIPanel*>(pUI);
		UI* pTemp = pSlot->FindChildUI(L"Picture");
		if (pTemp)
		{
			UIPanel* pPicture = static_cast<UIPanel*>(pTemp);
			Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player0.png");
			if (pBitmap)		pPicture->SetBitmap(pBitmap);
		}

		pUI = pSlot->FindChildUI(L"Nickname");
		if (pUI)
		{
			UIText* pTextNickname = static_cast<UIText*>(pUI);
			pTextNickname->ReassignText((wchar_t*)_packet);
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

void PacketHandler::S_Chat(char* _packet)
{
	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	eSessionState eState = pLobbyScene->GetState();

	if (eState == eSessionState::Lobby)
	{
		UI* pUI = UIManager::GetInst()->FindUI(L"Chat");
		if (pUI)
		{
			UIPanel* pPanel = static_cast<UIPanel*>(pUI);
			pUI = pPanel->FindChildUI(L"ChatList");
			UIScrollView* pScroll = static_cast<UIScrollView*>(pUI);
			std::wstring chat((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
			std::wstring nickname((wchar_t*)_packet);
			pScroll->AddItem(nickname + L" : " + chat, 20.f);
		}
	}
	else if (eState == eSessionState::WaitingRoom)
	{
		UI* pUI = UIManager::GetInst()->FindUI(L"WaitingRoomChat");
		if (pUI)
		{
			UIPanel* pPanel = static_cast<UIPanel*>(pUI);
			pUI = pPanel->FindChildUI(L"WaitingRoomChatList");
			UIScrollView* pScroll = static_cast<UIScrollView*>(pUI);
			std::wstring chat((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
			std::wstring nickname((wchar_t*)_packet);
			pScroll->AddItem(nickname + L" : " + chat, 20.f);
		}
	}

	Debug::Log("PacketHandler::S_Chat");
}

void PacketHandler::S_JoinRoom(char* _packet)
{
	UI* pUI = nullptr;

	std::wstring roomTitle((wchar_t*)_packet);					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
	pUI = UIManager::GetInst()->FindUI(L"RoomTitle");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"Text");
		if (pUI)
		{
			UIText* pText = static_cast<UIText*>(pUI);
			pText->ReassignText(roomTitle);
		}
	}

	int userCount = *(ushort*)_packet;				_packet += sizeof(ushort);

	for (int i = 0; i < userCount; i++)
	{
		char slotLocation = *(char*)_packet;				_packet += sizeof(char);
		u_int choice = *(char*)_packet;				_packet += sizeof(char);
		eMemberType eType = (eMemberType) * (char*)_packet;				_packet += sizeof(char);
		eMemberState eState = (eMemberState) * (char*)_packet;				_packet += sizeof(char);
		std::wstring nickname((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

		pUI = UIManager::GetInst()->FindUI(L"UserSlot" + std::to_wstring(slotLocation));
		if (pUI)
		{
			UIPanel* pSlot = static_cast<UIPanel*>(pUI);
			UI* pTemp = pSlot->FindChildUI(L"Picture");
			if (pTemp)
			{
				UIPanel* pPicture = static_cast<UIPanel*>(pTemp);
				Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player" + std::to_wstring(choice) + L".png");
				if (pBitmap)		pPicture->SetBitmap(pBitmap);
			}

			pUI = pSlot->FindChildUI(L"Nickname");
			if (pUI)
			{
				UIText* pTextNickname = static_cast<UIText*>(pUI);
				pTextNickname->ReassignText(nickname);
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
					if(eState == eMemberState::Wait)
						pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
					else if(eState == eMemberState::Ready)
						pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_ready.png");
				}
				if (pBitmap) 
					pTextState->SetBitmap(pBitmap);
			}
			pSlot->SetActive(true);
		}
	}

	Mouse* pMouse = InputManager::GetInst()->GetMouse();
	pMouse->SetActive(true);

	UIList* pList = nullptr;
	UIPage* pPage = nullptr;
	// UserList, Chat, Roomlist 전부 삭제
	pUI = UIManager::GetInst()->FindUI(L"Chat");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"ChatList");
		if (pUI)
		{
			UIScrollView* pScroll = static_cast<UIScrollView*>(pUI);
			pScroll->Clear();
		}
	}

	pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (pUI)
	{
		UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
		pUI = pUserListPanel->FindChildUI(L"UserList");
		if (pUI)
		{
			pPage = static_cast<UIPage*>(pUI);
			pPage->Reset();
		}
	}

	pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (pUI)
	{
		UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
		pUI = pRoomListPanel->FindChildUI(L"RoomList");
		if (pUI)
		{
			pPage = static_cast<UIPage*>(pUI);
			pPage->Reset();
		}
	}

	LobbyScene* pLobbyScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	pLobbyScene->ChangeSceneUI(eSessionState::WaitingRoom);

	pUI = UIManager::GetInst()->FindUI(L"StartGame");
	if (pUI)
	{
		UIButton* pBtn = static_cast<UIButton*>(pUI);
		pBtn->SetCallback(&LobbyScene::GameReadyCallback, pLobbyScene);
		UIText* pText = pBtn->GetUIText();
		pText->ReassignText(L"준비");
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
	u_int choice = *(char*)_packet;				_packet += sizeof(char);
	eMemberType eType = (eMemberType) * (ushort*)_packet;				_packet += sizeof(ushort);
	std::wstring nickname((wchar_t*)_packet);

	std::wstring uiName = L"UserSlot" + std::to_wstring(slotLocation);
	UI* pUI = UIManager::GetInst()->FindUI(uiName);
	if (pUI)
	{
		UIPanel* pSlot = static_cast<UIPanel*>(pUI);
		UI* pTemp = pSlot->FindChildUI(L"Picture");
		if (pTemp)
		{
			UIPanel* pPicture = static_cast<UIPanel*>(pTemp);
			Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player" + std::to_wstring(choice) + L".png");
			if (pBitmap)		pPicture->SetBitmap(pBitmap);
		}

		pUI = pSlot->FindChildUI(L"Nickname");
		if (pUI)
		{
			UIText* pTextNickname = static_cast<UIText*>(pUI);
			pTextNickname->ReassignText(nickname);
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
	pLobbyScene->ChangeSceneUI(eSessionState::Lobby);

	UI* pUI = UIManager::GetInst()->FindUI(L"WaitingRoomChat");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"WaitingRoomChatList");
		UIScrollView* pScroll = static_cast<UIScrollView*>(pUI);
		pScroll->Clear();
	}

	Debug::Log("PacketHandler::S_EnterLobby");
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
	UI* pUI = UIManager::GetInst()->FindUI(L"StartGame");
	if (!pUI) return;
	UIButton* pBtn = static_cast<UIButton*>(pUI);

	if (type == eMemberType::Owner)
	{
		pBtn->SetCallback(&LobbyScene::GameStartCallback, pLobbyScene);
		UIText* pText = pBtn->GetUIText();
		pText->ReassignText(L"게임 시작");
	}
	else if (type == eMemberType::Member)
	{
		pBtn->SetCallback(&LobbyScene::GameReadyCallback, pLobbyScene);
		UIText* pText = pBtn->GetUIText();
		pText->ReassignText(L"준비");
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
		pText->ReassignText(L"준비");
	}
	else if (state == eMemberState::Ready)
	{
		pText->ReassignText(L"대기");
	}

	Debug::Log("PacketHandler::S_UpdateWaitingRoomBtn");
}

void PacketHandler::S_InGameReady(char* _packet)
{
	int memberCount = *(char*)_packet;				_packet += sizeof(char);
	int mySlotNumber = *(char*)_packet;				_packet += sizeof(char); // 내 플레이어는 빨간색

	Player* myPlayer = nullptr;
	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	Layer* pLayer = pScene->FindLayer(L"Player");

	for (int i = 0; i < memberCount; i++)
	{
		int slot = *(char*)_packet;				_packet += sizeof(char);
		int characterChoice = *(char*)_packet;				_packet += sizeof(char);
		wchar_t* nickname = (wchar_t*)_packet;				_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

		myPlayer = new Player(pScene);
		ObjectManager::GetInst()->AddObj(myPlayer);
		
		myPlayer->SetName(std::to_wstring(slot));

		UI* pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
		if (!pUI) continue;
		pUI->SetActive(true);
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"Picture");
		UIPanel* pPicture = static_cast<UIPanel*>(pUI);
		Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player" + std::to_wstring(characterChoice) + L".png");
		pPicture->SetBitmap(pBitmap);

		pUI = pPanel->FindChildUI(L"Nickname");
		if (pUI)
		{
			UIText* pText = static_cast<UIText*>(pUI);
			pText->ReassignText(nickname);
			if (mySlotNumber == slot)
			{
				pText->SetTextColor(D2D1::ColorF::Red);
			}
		}

		AnimationClip* pClip = ResourceManager::GetInst()->GetAnimClip(L"player" + std::to_wstring(characterChoice), L"player" + std::to_wstring(characterChoice));
		if (pClip)
		{
			pClip->SetLoop(true);
			pClip->SetPlayTime(1.5f);
			pClip->SetAnyState(true);
			if (slot == 0 || slot == 2)
				pClip->SetFlip(true);
		}
		Animator* pAnimator = new Animator(pClip);

		/* 애니메이션 클립 등록 */
		pClip = ResourceManager::GetInst()->GetAnimClip(L"player" + std::to_wstring(characterChoice) + L"Walk", L"player" + std::to_wstring(characterChoice));
		if (pClip)
		{
			pClip->SetLoop(true);
			pClip->SetPlayTime(1.5f);
			pClip->SetAnyState(true);
			if (slot == 0 || slot == 2)
				pClip->SetFlip(true);
		}
		pAnimator->AddClip(L"Walk", pClip);

		pClip = ResourceManager::GetInst()->GetAnimClip(L"player" + std::to_wstring(characterChoice) + L"Hit", L"player" + std::to_wstring(characterChoice));
		if (pClip)
		{
			pClip->SetLoop(false);
			pClip->SetPlayTime(0.5f);
			pClip->SetAnyState(false);
			pClip->SetNextClip(pAnimator->GetDefaultClip());
			if (slot == 0 || slot == 2)
				pClip->SetFlip(true);
		}
		pAnimator->AddClip(L"Hit", pClip);

		/* 플레이어 위치 세팅 */
		if (slot == 0)
		{
			myPlayer->SetPos(240, 280);
			myPlayer->SetDir(eDir::Right);
		}
		else if(slot == 1) 
		{
			myPlayer->SetPos(1030, 280);
			myPlayer->SetDir(eDir::Left);
		}
		else if (slot == 2)
		{
			myPlayer->SetPos(240, 580);
			myPlayer->SetDir(eDir::Right);
		}
		else if (slot == 3)
		{
			myPlayer->SetPos(1030, 580);
			myPlayer->SetDir(eDir::Left);
		}

		myPlayer->SetNicknameUIText(nickname);
		myPlayer->SetRatio(1.5f);
		myPlayer->SetAnimator(pAnimator);

		pLayer->AddObj(myPlayer);
	}

	Debug::Log("PacketHandler::S_InGameReady");
}

void PacketHandler::S_UpdateUserListPage(char* _packet)
{
	int newPage = *(char*)_packet;			_packet += sizeof(char);
	int numOfUser = *(char*)_packet;			_packet += sizeof(char);

	if (numOfUser <= 0) return;

	UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
	if (!pUI)	return;
	UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
	pUI = pUserListPanel->FindChildUI(L"UserList");
	UIPage* pPage = static_cast<UIPage*>(pUI);
	
	u_int maxViewCount = pPage->GetMaxItemViewCount();

	pPage->SetCurPageIdx(newPage);
	UIList* pList = pPage->GetUIList();
	UIPanel* pPanel = nullptr;
	UIText* pText = nullptr;

	for (int i = 0; i < maxViewCount; i++)
	{
		pUI = pList->GetIdxItem(i);
		pPanel = static_cast<UIPanel*>(pUI);
		if(i >= numOfUser)
		{
			pPanel->SetActive(false);
			continue;
		}
		pPanel->SetActive(true);

		pUI = pPanel->FindChildUI(L"Nickname"); 
		if (pUI)
		{
			pText = static_cast<UIText*>(pUI);
			pText->ReassignText((wchar_t*)_packet);			
			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		}
		pUI = pPanel->FindChildUI(L"SessionState");
		if (pUI)
		{
			pText = static_cast<UIText*>(pUI);

			eSessionState eState = (eSessionState)*(char*)_packet;
			_packet += sizeof(char);

			if (eState == eSessionState::Lobby)
			{
				pText->ReassignText(sessionStateStr[*(char*)_packet]);							_packet += sizeof(char);
			}
			else
			{
				pText->ReassignText(std::to_wstring(*(u_int*)_packet) + L"번방");			_packet += sizeof(u_int);
			}
		}
	}
	Debug::Log("PacketHandler::S_UpdateUserListPage");
}

void PacketHandler::S_UpdateRoomListPage(char* _packet)
{
	int newPage = *(char*)_packet;			_packet += sizeof(char);
	int numOfRoom = *(char*)_packet;			_packet += sizeof(char);

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (!pUI) return;
	
	UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
	pUI = pRoomListPanel->FindChildUI(L"RoomList");
	if (!pUI) return;
	UIPage* pRoomPage = static_cast<UIPage*>(pUI);
	UIList* pRoomPageList = pRoomPage->GetUIList();

	pRoomPage->SetCurPageIdx(newPage);
	u_int maxViewCount = pRoomPage->GetMaxItemViewCount();

	for (int i = 0; i < maxViewCount; i++)
	{
		UI* pUI = pRoomPageList->GetIdxItem(i);
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		if (i >= numOfRoom)
		{
			pPanel->SetActive(false);
			continue;
		}
		pPanel->SetActive(true);

		u_int roomId = *(char*)_packet;					_packet += sizeof(u_int);
		ushort eRoomState = *(ushort*)_packet;					_packet += sizeof(ushort);
		std::wstring roomTitle = (wchar_t*)_packet;				_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		std::wstring  roomOwner = (wchar_t*)_packet;			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
		ushort userCount = *(wchar_t*)_packet;					_packet += sizeof(ushort);

		// TODO 로컬변수 만들지말고 바로 전달
		std::wstring strRoomId(std::to_wstring(roomId));
		std::wstring strRoomState(eRoomState == 1 ? L"Ready" : L"InGame");
		std::wstring strUserCount(std::to_wstring(userCount) + L" / 4");
		
		pPanel->SetActive(true);
		pPanel->SetName(strRoomId);
		UIButton* pBtn = static_cast<UIButton*>(pPanel->FindChildUI(L"Button"));
		if (pBtn)
		{
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
		}
		UIText* pText = static_cast<UIText*>(pPanel->FindChildUI(L"RoomId"));
		if(pText) pText->ReassignText(strRoomId);
		pText = static_cast<UIText*>(pPanel->FindChildUI(L"State"));
		if(pText) pText->ReassignText(strRoomState);
		pText = static_cast<UIText*>(pPanel->FindChildUI(L"Title"));
		if(pText) pText->ReassignText(roomTitle);
		pText = static_cast<UIText*>(pPanel->FindChildUI(L"Owner"));
		if(pText) pText->ReassignText(roomOwner);
		pText = static_cast<UIText*>(pPanel->FindChildUI(L"Count"));
		if(pText) pText->ReassignText(strUserCount);
	}
	Debug::Log("PacketHandler::S_UpdateRoomListPage");
}

void PacketHandler::S_UpdateUserSlot(char* _packet)
{
	u_int slotNumber = *(char*)_packet;					_packet += sizeof(char);
	u_int choice = *(char*)_packet;					_packet += sizeof(char);

	UI* pUI = UIManager::GetInst()->FindUI(L"UserSlot" + std::to_wstring(slotNumber));
	if (!pUI) return;

	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pUI = pPanel->FindChildUI(L"Picture");
	pPanel = static_cast<UIPanel*>(pUI);
	Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player" + std::to_wstring(choice) + L".png");
	if (pBitmap)		pPanel->SetBitmap(pBitmap);

	Debug::Log("PacketHandler::S_UpdateUserSlot");
}

void PacketHandler::S_Skill(char* _packet)
{
	u_int slotNumber = *(char*)_packet;					_packet += sizeof(char);
	eSkillType skillType = eSkillType(*(char*)_packet);					_packet += sizeof(char);


	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	pScene->ChangeState(eInGameState::UseSkill);
	if (skillType == eSkillType::None)
	{
		pScene->SetSkillState(eSkillState::End);
		return;
	}

	Obj* pObj = ObjectManager::GetInst()->FindObj(std::to_wstring(slotNumber));
	if (!pObj) return;

	Player* pPlayer = static_cast<Player*>(pObj);
	pPlayer->UseSkill(skillType);

	//if (skillType == eSkillType::LeftMove || skillType == eSkillType::LeftDoubleMove || skillType == eSkillType::RightMove || skillType == eSkillType::RightDoubleMove || skillType == eSkillType::DownMove || skillType == eSkillType::UpMove)

	pScene->SetSkillState(eSkillState::InUse);

	Debug::Log("PacketHandler::S_Skill");
}

void PacketHandler::S_UpdateTurn(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"SkillButtonPanel");
	if (pUI)
	{
		pUI->SetPos(ScreenWidth / 2, ScreenHeight);
		pUI->SetClickable(true);
	}
	pUI = UIManager::GetInst()->FindUI(L"Wait");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pPanel->SetActive(false);
	}

	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	pScene->SetMyTurn(true);
	pScene->ChangeState(eInGameState::Play);

	Debug::Log("PacketHandler::S_UpdateTurn");
}

void PacketHandler::S_UpdateDashboard(char* _packet)
{
	int curTurn = int(*(char*)_packet);			_packet += sizeof(char);

	UI* pUI = UIManager::GetInst()->FindUI(L"Dashboard");
	if (!pUI) return;

	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pUI = pPanel->FindChildUI(L"DashboardText");
	if (!pUI) return;

	UIText* pText = static_cast<UIText*>(pUI);
	pText->ReassignText(std::to_wstring(curTurn) + L" / " + std::to_wstring(GAME_MAX_TURN));

	Debug::Log("PacketHandler::S_UpdateDashboard");
}

void PacketHandler::S_GameOver(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"GameOver");
	if (!pUI) return;

	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pPanel->SetActive(true);
	UIManager::GetInst()->AddPopupUI(pPanel);

	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	pScene->ChangeState(eInGameState::GameOver);

	Debug::Log("PacketHandler::S_GameOver");
}

void PacketHandler::S_GameOverSceneChange(char* _packet)
{
	LobbyScene* pScene = SceneManager::GetInst()->GetCurScene<LobbyScene>();
	pScene->ChangeSceneUI(eSessionState::WaitingRoom);

	std::wstring roomTitle((wchar_t*)_packet);					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
	UI* pUI = UIManager::GetInst()->FindUI(L"RoomTitle");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"Text");
		if (pUI)
		{
			UIText* pText = static_cast<UIText*>(pUI);
			pText->ReassignText(roomTitle);
		}
	}

	int userCount = *(ushort*)_packet;				_packet += sizeof(ushort);

	for (int i = 0; i < userCount; i++)
	{
		char slotLocation = *(char*)_packet;				_packet += sizeof(char);
		u_int choice = *(char*)_packet;				_packet += sizeof(char);
		eMemberType eType = (eMemberType) * (ushort*)_packet;				_packet += sizeof(ushort);
		std::wstring nickname((wchar_t*)_packet);			_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

		pUI = UIManager::GetInst()->FindUI(L"UserSlot" + std::to_wstring(slotLocation));
		if (pUI)
		{
			UIPanel* pSlot = static_cast<UIPanel*>(pUI);
			UI* pTemp = pSlot->FindChildUI(L"Picture");
			if (pTemp)
			{
				UIPanel* pPicture = static_cast<UIPanel*>(pTemp);
				Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player" + std::to_wstring(choice) + L".png");
				if (pBitmap)		pPicture->SetBitmap(pBitmap);
			}

			pUI = pSlot->FindChildUI(L"Nickname");
			if (pUI)
			{
				UIText* pTextNickname = static_cast<UIText*>(pUI);
				pTextNickname->ReassignText(nickname);
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

	Debug::Log("PacketHandler::S_GameOverSceneChange");
}

void PacketHandler::S_UpdateIngameUserLeave(char* _packet)
{
	int slot = int(*(char*)_packet);				_packet += sizeof(char);
	eSkillType type = eSkillType(*(char*)_packet);

	UI* pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
	if (!pUI) return;
	pUI->SetActive(false);
	
	//Obj* pObj = ObjectManager::GetInst()->FindObj(std::to_wstring(slot));
	//Player* pPlayer = static_cast<Player*>(pObj);

	ObjectManager::GetInst()->KillObj(std::to_wstring(slot));
	Skill* pSkill = ObjectManager::GetInst()->FindSkill(type);
	if(pSkill)
		pSkill->Reset();

	// 나간 플레이어가 스킬 사용 중일때

	Debug::Log("PacketHandler::S_UpdateIngameUserLeave");
}

void PacketHandler::S_Standby(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"Standby");
	if (!pUI) return;
	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pPanel->SetActive(true);
	UIManager::GetInst()->AddPopupUI(pPanel);
	
	// InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();

	Debug::Log("PacketHandler::S_Standby");
}

void PacketHandler::S_UpdateProfile(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"Profile");
	if (!pUI) return;
	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pUI = pPanel->FindChildUI(L"ProfileText");
	UIText* pText = static_cast<UIText*>(pUI);
	pText->ReassignText((wchar_t*)_packet);					_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;
	pUI = pPanel->FindChildUI(L"HitCountText");
	pText = static_cast<UIText*>(pUI);
	pText->ReassignText(L"맞은 횟수 : " + std::to_wstring(*(u_int*)_packet));

	Debug::Log("PacketHandler::S_UpdateProfile");
}

void PacketHandler::S_CheckHit(char* _packet)
{
	int playerSize = *(char*)_packet;			_packet += sizeof(char);
	int slot = 0;
	int score = 0;
	eSkillType eType = eSkillType::None;
	UI* pUI = nullptr;
	UIPanel* pPanel = nullptr;
	UIText* pText = nullptr;

	for (int i = 0; i < playerSize; ++i)
	{
		slot = *(char*)_packet;				_packet += sizeof(char);
		score = *(char*)_packet;			_packet += sizeof(char);
		eType = eSkillType(*(char*)_packet);			_packet += sizeof(char);

		pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
		if (!pUI) continue;

		pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"Score");
		if (!pUI) continue;

		pText = static_cast<UIText*>(pUI);
		pText->ReassignText(L"두들겨 맞은 횟수 : " + std::to_wstring(score));

		if (eType != eSkillType::Shield)
		{
			Obj* pObj = ObjectManager::GetInst()->FindObj(std::to_wstring(slot));
			if (pObj)
			{
				Player* pPlayer = static_cast<Player*>(pObj);
				pPlayer->UseSkill(eSkillType::Hit);
			}
		}
	}

	if (playerSize == 0)
	{
		InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
		pScene->SetSkillState(eSkillState::End);
	}

	Debug::Log("PacketHandler::S_CheckHit");
}

