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
#include "../Managers/SkillManager.h"
#include "../Skill/SkillInfo.h"
#include "../Skill/SkillAttack.h"
#include "../Skill/SkillHeal.h"
#include "../Managers/InputManager.h"
#include "../Animation/AnimationClip.h"
#include "../Animation/Animator.h"
#include "../Obj/GameObj/Player.h"
#include "../Obj/GameObj/Skill.h"
#include "../Debug.h"
#include "../Constants.h"
#include "../Game.h"
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
		UIManager::GetInst()->SetPopupUI(static_cast<UIPanel*>(pUI));
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
		Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_ready_normal.png");
		pBtn->SetBitmap(pBitmap);
		pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_ready_pressed.png");
		pBtn->SetPressedBitmap(pBitmap);
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
		UIManager::GetInst()->SetPopupUI(static_cast<UIPanel*>(pUI));
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
		UIManager::GetInst()->SetPopupUI(static_cast<UIPanel*>(pUI));
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
		Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_start_normal.png");
		pBtn->SetBitmap(pBitmap);
		pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_start_pressed.png");
		pBtn->SetPressedBitmap(pBitmap);
	}
	else if (type == eMemberType::Member)
	{
		pBtn->SetCallback(&LobbyScene::GameReadyCallback, pLobbyScene);
		Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_ready_normal.png");
		pBtn->SetBitmap(pBitmap);
		pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_ready_pressed.png");
		pBtn->SetPressedBitmap(pBitmap);
	}

	Debug::Log("PacketHandler::S_UpdateUserType");
}

void PacketHandler::S_UpdateWaitingRoomBtn(char* _packet)
{
	eMemberState state = (eMemberState)*(char*)_packet;
	UI* pUI = UIManager::GetInst()->FindUI(L"StartGame");
	if (!pUI) return;
	UIButton* pBtn = static_cast<UIButton*>(pUI);
	if (state == eMemberState::Wait)
	{
		Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_ready_normal.png");
		pBtn->SetBitmap(pBitmap);
		pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_ready_pressed.png");
		pBtn->SetPressedBitmap(pBitmap);

	}
	else if (state == eMemberState::Ready)
	{
		Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_wait_normal.png");
		pBtn->SetBitmap(pBitmap);
		pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_wait_pressed.png");
		pBtn->SetPressedBitmap(pBitmap);
	}

	Debug::Log("PacketHandler::S_UpdateWaitingRoomBtn");
}

void PacketHandler::S_InGameReady(char* _packet)
{
	int memberCount = *(char*)_packet;				_packet += sizeof(char);
	int mySlotNumber = *(char*)_packet;				_packet += sizeof(char); // 내 플레이어는 빨간색

	Game::GetInst()->SetMySlot(mySlotNumber);

	Player* myPlayer = nullptr;
	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	Layer* pLayer = pScene->FindLayer(L"Player");
	Layer* pNicknameLayer = pScene->FindLayer(L"PlayerNickname");

	for (int i = 0; i < memberCount; i++)
	{
		int slot = *(char*)_packet;							_packet += sizeof(char);
		int characterChoice = *(char*)_packet;				_packet += sizeof(char);
		wchar_t* nickname = (wchar_t*)_packet;				_packet += (ushort)wcslen((wchar_t*)_packet) * 2 + 2;

		myPlayer = new Player(pScene, slot);
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

		pClip = ResourceManager::GetInst()->GetAnimClip(L"player" + std::to_wstring(characterChoice) + L"Die", L"player" + std::to_wstring(characterChoice));
		if (pClip)
		{
			pClip->SetLoop(false);
			pClip->SetPlayTime(1.5f);
			pClip->SetAnyState(false);
			pClip->SetNextClip(pAnimator->GetDefaultClip());
			if (slot == 0 || slot == 2)
				pClip->SetFlip(true);
		}
		pAnimator->AddClip(L"Die", pClip);

		Game::GetInst()->AddPlayer(slot);

		/* 플레이어 위치 세팅 */
		if (slot == 0)
		{
			myPlayer->SetPos(240, 220);
			myPlayer->SetBoardPos(0, 0);
			myPlayer->SetDir(eDir::Right);
		}
		else if(slot == 1) 
		{
			myPlayer->SetPos(1030, 220);
			myPlayer->SetBoardPos(4, 0);
			myPlayer->SetDir(eDir::Left);
		}
		else if (slot == 2)
		{
			myPlayer->SetPos(240, 580);
			myPlayer->SetBoardPos(0, 3);
			myPlayer->SetDir(eDir::Right);
		}
		else if (slot == 3)
		{
			myPlayer->SetPos(1030, 580);
			myPlayer->SetBoardPos(4, 3);
			myPlayer->SetDir(eDir::Left);
		}

		UIPanel* nicknameUI = myPlayer->SetNicknameUI(nickname);
		pNicknameLayer->AddObj(nicknameUI);
		myPlayer->SetRatio(1.5f);
		myPlayer->SetAnimator(pAnimator);

		pLayer->AddObj(myPlayer);
	}

	Debug::Log("PacketHandler::S_InGameReady");
}

void PacketHandler::S_UpdateUserListPage(char* _packet)
{
	int newPage = *(char*)_packet;			_packet += sizeof(char);
	u_int numOfUser = *(char*)_packet;			_packet += sizeof(char);

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

	for (u_int i = 0; i < maxViewCount; i++)
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
	u_int numOfRoom = *(char*)_packet;			_packet += sizeof(char);

	UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
	if (!pUI) return;
	
	UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
	pUI = pRoomListPanel->FindChildUI(L"RoomList");
	if (!pUI) return;
	UIPage* pRoomPage = static_cast<UIPage*>(pUI);
	UIList* pRoomPageList = pRoomPage->GetUIList();

	pRoomPage->SetCurPageIdx(newPage);
	u_int maxViewCount = pRoomPage->GetMaxItemViewCount();

	for (u_int i = 0; i < maxViewCount; i++)
	{
		UI* pUI = pRoomPageList->GetIdxItem(i);
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		if (i >= numOfRoom)
		{
			pPanel->SetActive(false);
			continue;
		}
		pPanel->SetActive(true);

		u_int roomId = *(char*)_packet;							_packet += sizeof(u_int);
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
					char buffer[255] = {};
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
	eActionType actionType = eActionType(*(char*)_packet);					_packet += sizeof(char);

	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	pScene->ChangeState(eInGameState::UseSkill);
	if (actionType == eActionType::None)
	{
		pScene->SetSkillState(eSkillState::End);
		return;
	}

	Obj* pObj = ObjectManager::GetInst()->FindObj(std::to_wstring(slotNumber));
	if (!pObj) return;

	Player* pPlayer = static_cast<Player*>(pObj);

	if (actionType == eActionType::Move)
	{
		eMoveName name = Game::GetInst()->Move(slotNumber, eMoveName(*(char*)_packet));
		if(name == eMoveName::None)
		{
			Game::GetInst()->CheckPortal(slotNumber);
			pScene->SetSkillState(eSkillState::End);
			return;
		}
		pPlayer->DoAction(name);
	}
	else if (actionType == eActionType::Skill)
	{
		eSkillName skillName = eSkillName(*(char*)_packet);
		Game::GetInst()->SetSkillName(slotNumber, skillName);
		pPlayer->DoAction(skillName);

		switch (skillName)
		{
		case eSkillName::Attack0:
		case eSkillName::Attack0_Left:
		case eSkillName::Attack1:
		case eSkillName::Attack2:
		case eSkillName::Attack3:
		case eSkillName::Attack4:
		{
			int xpos = 0, ypos = 0;
			const SkillInfo* pSkill = SkillManager::GetInst()->GetSkill(slotNumber, skillName);
			int mana = Game::GetInst()->GetMana(slotNumber) - pSkill->GetMana();
			Game::GetInst()->SetMana(slotNumber, mana);

			pScene->UpdateMPUI(slotNumber, mana);

			const SkillAttack* pAttack = dynamic_cast<const SkillAttack*>(pSkill);
			if (pAttack)
			{
				const std::vector<std::pair<int, int>>& list = pAttack->GetListCoordinates();

				UI* pUI = UIManager::GetInst()->FindUI(L"RangeBlock");
				pUI->UI::SetActive(true);
				UIPanel* pPanel = static_cast<UIPanel*>(pUI);

				int slotXPos = Game::GetInst()->GetSlotXPos(slotNumber);
				int slotYPos = Game::GetInst()->GetSlotYPos(slotNumber);

				for (const auto& coor : list)
				{
					xpos = slotXPos + coor.first;
					ypos = slotYPos + coor.second;
					if (xpos < 0 || ypos < 0 || xpos >= Game::BoardWidth || ypos >= Game::BoardHeight)
						continue;
					pUI = pPanel->FindChildUI(std::to_wstring((ypos * 10) + xpos));
					pUI->SetActive(true);
				}
			}
		}
		break;
		}
	}


	Debug::Log("PacketHandler::S_Skill");
}

void PacketHandler::S_UpdateTurn(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"SkillButtonPanel");
	if (pUI)
	{
		pUI->SetPos(ScreenWidth / 2.f, ScreenHeight);
		pUI->SetClickable(true);

		UIPanel* pPanel = static_cast<UIPanel*>(pUI);

		int skillNameListSize = *(char*)_packet;				_packet += sizeof(char);
		for (int i = 0; i < skillNameListSize; i++)
		{
			eSkillName name = (eSkillName)*(char*)_packet;				_packet += sizeof(char);
			pUI = pPanel->FindChildUI(std::to_wstring((int)name));				
			if(pUI)
				pUI->SetActive(false);
		}
	}

	pUI = UIManager::GetInst()->FindUI(L"Wait");
	if (pUI)
	{
		UIPanel* pPanel = static_cast<UIPanel*>(pUI);
		pPanel->SetActive(false);
	}

	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	pScene->ChangeState(eInGameState::Play);

	Debug::Log("PacketHandler::S_UpdateTurn");
}

void PacketHandler::S_UpdateDashboard(char* _packet)
{
	int curTurn = int(*(char*)_packet);			_packet += sizeof(char);

	if (curTurn > Game::GameMaxRound) return;

	UI* pUI = UIManager::GetInst()->FindUI(L"Dashboard");
	if (!pUI) return;

	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pUI = pPanel->FindChildUI(L"DashboardText");
	if (!pUI) return;

	UIText* pText = static_cast<UIText*>(pUI);
	pText->ReassignText(std::to_wstring(curTurn) + L" / " + std::to_wstring(Game::GameMaxRound));

	Debug::Log("PacketHandler::S_UpdateDashboard");
}

void PacketHandler::S_GameOver(char* _packet)
{
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
	// 누가 나갔고, 바뀐 방장 알려줌
	int slot = int(*(char*)_packet);				_packet += sizeof(char);
	eSkillName name = Game::GetInst()->GetSlotSkillName(slot);

	UI* pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
	if (!pUI) return;
	pUI->SetActive(false);

	// 나간애가 자기 차례라서 스킬 사용 중일때 스킬 종료하고 다음 차례
	if (slot == Game::GetInst()->GetCurSlot())
	{
		ObjectManager::GetInst()->KillObj(std::to_wstring(slot));
		Skill* pSkill = ObjectManager::GetInst()->FindSkill(name);
		if (pSkill)
			pSkill->Reset();
	}

	pUI = UIManager::GetInst()->FindUI(L"RangeBlock");
	UIPanel* pPanel = static_cast<UIPanel*>(pUI);
	pPanel->SetActive(false);

	Game::GetInst()->LeavePlayer(slot);

	// 2명인데 한 명이 나간경우
	if (Game::GetInst()->GetPlayingCount() <= 1)
	{
		InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
		pScene->ChangeState(eInGameState::GameOver);
	}

	// 나간애가 방장일때
	//if(Game::GetInst()->)

	

	Debug::Log("PacketHandler::S_UpdateIngameUserLeave");
}

void PacketHandler::S_Standby(char* _packet)
{
	UI* pUI = UIManager::GetInst()->FindUI(L"Standby");
	if (!pUI) return;
	UIManager::GetInst()->SetPopupUI(static_cast<UIPanel*>(pUI));

	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	pScene->ChangeState(eInGameState::Prepare);

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
	pUI = pPanel->FindChildUI(L"KillCountText");
	pText = static_cast<UIText*>(pUI);
	pText->ReassignText(std::to_wstring(*(u_int*)_packet) + L" 킬");

	Debug::Log("PacketHandler::S_UpdateProfile");
}

void PacketHandler::S_CheckHit(char* _packet)
{
	int playerSize = *(char*)_packet;			_packet += sizeof(char);
	int slot = 0;
	int hp = 0;
	//eActionType eType = eActionType::None;
	UI* pUI = nullptr;
	UIPanel* pPanel = nullptr;
	UIText* pText = nullptr;

	for (int i = 0; i < playerSize; ++i)
	{
		slot = *(char*)_packet;				_packet += sizeof(char);
		hp = *(char*)_packet;			_packet += sizeof(char);

		pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
		if (!pUI) continue;

		pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"HP");
		if (!pUI) continue;

		pText = static_cast<UIText*>(pUI);
		pText->ReassignText(L"HP : " + std::to_wstring(hp));

		Obj* pObj = ObjectManager::GetInst()->FindObj(std::to_wstring(slot));
		if (pObj)
		{
			Player* pPlayer = static_cast<Player*>(pObj);
			pPlayer->DoAction(eActionType::Hit);
		}
	}

	int deadPlayerSize = *(char*)_packet;			_packet += sizeof(char);
	for (int i = 0; i < deadPlayerSize; ++i)
	{
		slot = *(char*)_packet;				_packet += sizeof(char);

		pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
		if (!pUI) continue;

		pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"HP");
		if (!pUI) continue;

		pText = static_cast<UIText*>(pUI);
		pText->ReassignText(L"HP : " + std::to_wstring(0));

		Obj* pObj = ObjectManager::GetInst()->FindObj(std::to_wstring(slot));
		if (pObj)
		{
			Player* pPlayer = static_cast<Player*>(pObj);
			pPlayer->DoAction(eActionType::Die);
		}
		
		Game::GetInst()->DecreaseAliveCount();
	}

	if (playerSize == 0)
	{
		InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
		pScene->SetSkillState(eSkillState::End);
	}

	pUI = UIManager::GetInst()->FindUI(L"RangeBlock");
	pPanel = static_cast<UIPanel*>(pUI);
	pPanel->SetActive(false);

	Debug::Log("PacketHandler::S_CheckHit");
}

void PacketHandler::S_UpdateHeal(char* _packet)
{
	bool healPossible = bool(*(char*)_packet);			_packet += sizeof(char);
	if (healPossible)
	{
		int slot = int(*(char*)_packet);			_packet += sizeof(char);
		int mana = int(*(char*)_packet);			_packet += sizeof(char);
		
		UI* pUI = UIManager::GetInst()->FindUI(L"PlayerStat" + std::to_wstring(slot));
		if (!pUI) return;

		UIPanel *pPanel = static_cast<UIPanel*>(pUI);
		pUI = pPanel->FindChildUI(L"MP");
		if (!pUI) return;

		UIText* pText = static_cast<UIText*>(pUI);
		pText->ReassignText(L"MP : " + std::to_wstring(mana));
	}

	InGameScene* pScene = SceneManager::GetInst()->GetCurScene<InGameScene>();
	pScene->SetSkillState(eSkillState::End);

	Debug::Log("PacketHandler::S_UpdateHeal");
}

void PacketHandler::S_CreatePortal(char* _packet)
{
	int xpos = *(char*)_packet;					_packet += sizeof(char);
	int ypos = *(char*)_packet;					_packet += sizeof(char);
	int teleportX = *(char*)_packet;					_packet += sizeof(char);
	int teleportY = *(char*)_packet;					_packet += sizeof(char);

	Obj* pObj = ObjectManager::GetInst()->FindObj(L"Portal");
	pObj->SetPos(285 + (xpos * Player::LeftRightMoveDist), 300 + (ypos * Player::UpDownMoveDist));
	pObj->SetActive(true);

	Game::GetInst()->CreatePortal(xpos, ypos, teleportX, teleportY);

	Debug::Log("PacketHandler::S_CreatePortal");
}

void PacketHandler::S_Teleport(char* _packet)
{
	Obj* pObj = ObjectManager::GetInst()->FindObj(L"Portal");
	pObj->SetActive(false);

	int slot = *(char*)_packet;				_packet += sizeof(char);
	int xpos = *(char*)_packet;				_packet += sizeof(char);
	int ypos = *(char*)_packet;				_packet += sizeof(char);

	pObj = ObjectManager::GetInst()->FindObj(std::to_wstring(slot));

	int slotXPos = 240;
	int slotYPos = 220;
	if (slot == 2 || slot == 3)
		slotYPos = 280;

	if (slot == 1 || slot == 3)
		slotXPos = 310;

	// Player의 Move상태가 None이어야 함
	pObj->SetPos(slotXPos + (xpos * Player::LeftRightMoveDist), slotYPos + (ypos * Player::UpDownMoveDist));
	Player* pPlayer = static_cast<Player*>(pObj);
	if(pPlayer->GetActionType() != eActionType::None)
		pPlayer->SkillEnd(eSkillState::End);
	pPlayer->SetBoardPos(xpos, ypos);

	Debug::Log("PacketHandler::S_Teleport (slot : " + std::to_string(slot) + ", xpos : " + std::to_string(xpos) + ", ypos : " + std::to_string(ypos) + ")");
}

void PacketHandler::S_ExitInGame(char* _packet)
{
	Game::GetInst()->OnGameOver();

	LobbyScene* pScene = new LobbyScene;
	SceneManager::GetInst()->ChangeScene(pScene);
	pScene->ChangeSceneUI(eSessionState::Lobby);

	Debug::Log("PacketHandler::S_ExitInGame");
}

