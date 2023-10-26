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
#include "../Obj/MyPlayer.h"
#include "../Debug.h"
#include "../Constants.h"
//#include "../InstructionQueue.h"

typedef unsigned short ushort;

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

	std::wstring roomTitle((wchar_t*)_packet);
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

	pLobbyScene->ChangeSceneUI(eSessionState::WatingRoom);

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
			pTextNickname->ReassignText(strNickname);
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
	else if (eState == eSessionState::WatingRoom)
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
	pLobbyScene->ChangeSceneUI(eSessionState::WatingRoom);

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
	if (type == eMemberType::Owner)
	{
		UI* pUI = UIManager::GetInst()->FindUI(L"StartGame");
		if (!pUI) return;
		UIButton* pBtn = static_cast<UIButton*>(pUI);
		pBtn->SetCallback(&LobbyScene::GameStartCallback, pLobbyScene);
		UIText* pText = pBtn->GetUIText();
		pText->ReassignText(L"게임 시작");
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
			pText->ReassignText((wchar_t*)_packet);
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

void PacketHandler::S_UpdateUserListPage(char* _packet)
{
	int numOfUser = *(char*)_packet;			_packet += sizeof(char);
	int newPage = *(char*)_packet;			_packet += sizeof(char);

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

			if(eState == eSessionState::Lobby)
				pText->ReassignText(sessionStateStr[*(char*)_packet]);				
			else
				pText->ReassignText(std::to_wstring(*(char*)_packet) + L"번방");
			_packet += sizeof(char);
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

		unsigned int roomId = *(char*)_packet;					_packet += sizeof(char);
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

