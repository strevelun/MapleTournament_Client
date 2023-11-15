#include "../NetworkCore/NetworkManager.h"
#include "LobbyScene.h"
#include "../Managers/UIManager.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ObjectManager.h"
#include "../Managers/SceneManager.h"
#include "../Bitmap.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIScrollView.h"
#include "../Obj/UI/UIButton.h"
#include "../Obj/UI/UIEditText.h"
#include "../Obj/UI/UIList.h"
#include "../Obj/UI/UIText.h"
#include "../Obj/UI/UIPage.h"
#include "../Obj/Player.h"
#include "../Setting.h"
#include "../Constants.h"
#include "../Debug.h"
#include "../Timer.h"
#include "Layer.h"

#include <algorithm>

typedef unsigned short ushort;

LobbyScene::LobbyScene()
{
}

LobbyScene::~LobbyScene()
{
}

// LoginScene에서 LobbyScene으로 오는 경우 : ShowLobbyUI
// InGameScene에서 LobbyScene으로 오는 경우 : ShowWaitingRoomUI
bool LobbyScene::Init()
{
    if (!InitLobbyUI()) return false;
    if (!InitWaitingRoomUI()) return false;

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_LobbyInit;				count += sizeof(ushort);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);

    return true;
}

void LobbyScene::CreateRoomButtonCallback(UIEditText* _pEditText, const std::wstring& _str)
{
    if (_str.length() <= 0) return;
    if (std::all_of(_str.begin(), _str.end(), [](wchar_t c) { return c == L' '; })) { return; }
    if (!_pEditText) return;

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_CreateRoom;				count += sizeof(ushort);
    const wchar_t* str = _str.c_str();
    memcpy(buffer + count, str, wcslen(str) * 2);			                    count += (ushort)wcslen(str) * 2;
    *(wchar_t*)(buffer + count) = L'\0';								        count += 2;
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);

    UIManager::GetInst()->PopPopupUI();

    _pEditText->ClearEditText();

    UI* pUI = UIManager::GetInst()->FindUI(L"Chat");
    UIPanel* pPanel = static_cast<UIPanel*>(pUI);
    pUI = pPanel->FindChildUI(L"ChatList");
    UIScrollView* pScroll = static_cast<UIScrollView*>(pUI);
    pScroll->Clear();
}

void LobbyScene::ChatCallback(UIEditText* _pEditText, const std::wstring& _str)
{
    if (_str.length() <= 0) return;
    if (std::all_of(_str.begin(), _str.end(), [](wchar_t c) { return c == L' '; })) { return; }
    if (!_pEditText) return;

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_Chat;				count += sizeof(ushort);
    const wchar_t* str = _str.c_str();
    memcpy(buffer + count, str, wcslen(str) * 2);			                    count += (ushort)wcslen(str) * 2;
    *(wchar_t*)(buffer + count) = L'\0';								        count += 2;
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);

    _pEditText->ClearEditText();    
}

void LobbyScene::UpdateUserListPage()
{
    UI* pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
    if (!pUI) return;

    UIPanel* pUserListPanel = static_cast<UIPanel*>(pUI);
    pUI = pUserListPanel->FindChildUI(L"UserList");
    UIPage* pPage = static_cast<UIPage*>(pUI);
    char page = pPage->GetCurPageIdx();

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_UpdateUserListPage;		count += sizeof(ushort);
    *(char*)(buffer + count) = page;                                         count += sizeof(char);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void LobbyScene::UpdateRoomListPage()
{
    UI* pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
    if (!pUI) return;

    UIPanel* pRoomListPanel = static_cast<UIPanel*>(pUI);
    pUI = pRoomListPanel->FindChildUI(L"RoomList");
    UIPage* pPage = static_cast<UIPage*>(pUI);
    char page = pPage->GetCurPageIdx();

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_UpdateRoomListPage;		count += sizeof(ushort);
    *(char*)(buffer + count) = page;                                         count += sizeof(char);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void LobbyScene::UpdateUserSlot(u_int _choice)
{
    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_UpdateUserSlot;		count += sizeof(ushort);
    *(char*)(buffer + count) = (char)_choice;                                         count += sizeof(char);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

bool LobbyScene::ShowLobbyUI()
{
    Layer* pLayer = FindLayer(L"Lobby_Background");
    pLayer->SetActive(true);
    pLayer = FindLayer(L"Lobby_UI");
    pLayer->SetActive(true);

    return true;
}

bool LobbyScene::ShowWaitingRoomUI()
{
    Layer* pLayer = FindLayer(L"WaitingRoom_Background");
    pLayer->SetActive(true);
    pLayer = FindLayer(L"WaitingRoom_UI");
    pLayer->SetActive(true);
    
    return true;
}

bool LobbyScene::HideLobbyUI()
{
    Layer* pLayer = FindLayer(L"Lobby_Background");
    pLayer->SetActive(false);
    pLayer = FindLayer(L"Lobby_UI");
    pLayer->SetActive(false);

    return true;
}

bool LobbyScene::HideWaitingRoomUI()
{
    Layer* pLayer = FindLayer(L"WaitingRoom_Background");
    pLayer->SetActive(false);
    pLayer = FindLayer(L"WaitingRoom_UI");
    pLayer->SetActive(false);

    UI* pUI = UIManager::GetInst()->FindUI(L"UserSlot0");
    if (!pUI) return false;
    UIPanel* pSlot01 = static_cast<UIPanel*>(pUI);
    pSlot01->SetActive(false);
    pUI = UIManager::GetInst()->FindUI(L"UserSlot1");
    if (!pUI) return false;
    UIPanel* pSlot02 = static_cast<UIPanel*>(pUI);
    pSlot02->SetActive(false);
    pUI = UIManager::GetInst()->FindUI(L"UserSlot2");
    if (!pUI) return false;
    UIPanel* pSlot03 = static_cast<UIPanel*>(pUI);
    pSlot03->SetActive(false);
    pUI = UIManager::GetInst()->FindUI(L"UserSlot3");
    if (!pUI) return false;
    UIPanel* pSlot04 = static_cast<UIPanel*>(pUI);
    pSlot04->SetActive(false);

    return true;
}

bool LobbyScene::InitLobbyUI()
{
    Layer* pLayer = CreateLayer(L"Lobby_Background", 0, false);

    /* Background */
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_background2.png");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    pBackground->SetName(L"Background");
    UIManager::GetInst()->AddUI(pBackground);
    pLayer->AddObj(pBackground);

    pLayer = CreateLayer(L"Lobby_UI", 1, false);

    /* 방만들기 버튼 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_createroom_normal.png");
    if (!pBitmap) return false;
    UIButton* pRoomBtn = new UIButton(nullptr, 150, 100, pBackground->GetWidth() / 2, pBackground->GetHeight(), 0.5f, 1.0f);
    pRoomBtn->SetName(L"MakeRoom");
    pRoomBtn->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_createroom_pressed.png");
    if (!pBitmap) return false;
    pRoomBtn->SetPressedBitmap(pBitmap);
    pRoomBtn->SetCallback([]() 
        {
            UIManager::GetInst()->SetPopupUI(static_cast<UIPanel*>(UIManager::GetInst()->FindUI(L"CreateRoom")));
        });
    pRoomBtn->SetClickable(true);
    UIManager::GetInst()->AddUI(pRoomBtn);
    pLayer->AddObj(pRoomBtn);

    /* 방만들기 팝업 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobby_createroom.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(nullptr, 430, 640, ScreenWidth / 2, 0, 0.5f);
    pPanel->SetName(L"CreateRoom");
    pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddPopupUI(pPanel);
    //pLayer->AddObj(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pInputRoomName = new UIEditText(pPanel, pPanel->GetWidth() - 100, 50, 15, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2+40, 0.5f, 0.5f);
    pInputRoomName->SetBitmap(pBitmap);
    pInputRoomName->SetClickable(true);
    pInputRoomName->SetCallback(&LobbyScene::CreateRoomButtonCallback, this);
    pPanel->AddChildUI(pInputRoomName);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_normal.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2 - 10, pPanel->GetHeight() - 70, 1.0f, 1.f);
    pOKButton->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_pressed.png");
    if (!pBitmap) return false;
    pOKButton->SetPressedBitmap(pBitmap);
    pOKButton->SetCallback([this, pPanel, pInputRoomName]()
        {
            CreateRoomButtonCallback(pInputRoomName, pInputRoomName->GetText());
        });
    pOKButton->SetClickable(true);
    pPanel->AddChildUI(pOKButton);
    pPanel->SetActive(false);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_cancel_normal.png");
    if (!pBitmap) return false;
    UIButton* pCancelButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2 + 10, pPanel->GetHeight() - 70, 0.f, 1.f);
    pCancelButton->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_cancel_pressed.png");
    if (!pBitmap) return false;
    pCancelButton->SetPressedBitmap(pBitmap);
    pCancelButton->SetCallback([pInputRoomName]()
        {
            pInputRoomName->ClearEditText();
            UIManager::GetInst()->PopPopupUI();
        });
    pCancelButton->SetClickable(true);
    pPanel->AddChildUI(pCancelButton);

    /* Room List */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_roomlist.png");
    if (!pBitmap) return false;
    UIPanel* pRoomListPanel = new UIPanel(nullptr, 800, 400);
    pRoomListPanel->SetName(L"RoomListPanel");
    pRoomListPanel->SetBitmap(pBitmap);
    UIPage* pRoomPage = new UIPage(pRoomListPanel, 645, 310, 645, 25, 80, 45, 0.f, 0.f, 6.f);
    pRoomPage->SetName(L"RoomList");
    pRoomListPanel->AddChildUI(pRoomPage);
    UIManager::GetInst()->AddUI(pRoomListPanel);
    pLayer->AddObj(pRoomListPanel);

    /* Room List Item */
    pPanel = new UIPanel(pRoomPage, 645,26);
    UIButton* pBtn = new UIButton(pPanel, 645, 25);
    pBtn->SetName(L"Button");
    pBtn->SetClickable(true);
    pPanel->AddChildUI(pBtn);
    UIText* pTextRoomId = new UIText(pPanel, L"", 15.f, 0.f, 0.f);
    pTextRoomId->SetName(L"RoomId");
    pPanel->AddChildUI(pTextRoomId);
    UIText* pTextState = new UIText(pPanel, L"", 15.f, 50.f, 0.f);
    pTextState->SetName(L"State");
    pPanel->AddChildUI(pTextState);
    UIText* pTextTitle = new UIText(pPanel, L"", 20.f, 270.f, 0.f, 0.5f);
    pTextTitle->SetName(L"Title");
    pPanel->AddChildUI(pTextTitle);
    UIText* pTextOwner = new UIText(pPanel, L"", 20.f, 440.f, 0.f);
    pTextOwner->SetName(L"Owner");
    pPanel->AddChildUI(pTextOwner);
    UIText* pTextParticipants = new UIText(pPanel, L"", 20.f, 590.f, 0.f);
    pTextParticipants->SetName(L"Count");
    pPanel->AddChildUI(pTextParticipants);
    pRoomPage->SetItemTemplate(pPanel);
    pRoomPage->SetPrevBtnCallback([pRoomPage]()
        {
            u_int curPage = pRoomPage->GetCurPageIdx();
            if (curPage <= 0)
                return;

            char buffer[255];
            u_short count = sizeof(u_short);
            *(u_short*)(buffer + count) = (u_short)ePacketType::C_UpdateRoomListPage;		count += sizeof(u_short);
            *(char*)(buffer + count) = (char)curPage - 1;										count += sizeof(char);
            *(u_short*)buffer = count;
            NetworkManager::GetInst()->Send(buffer);
        });
    pRoomPage->SetNextBtnCallback([pRoomPage]()
        {
            u_int curPage = pRoomPage->GetCurPageIdx();

            char buffer[255];
            u_short count = sizeof(u_short);
            *(u_short*)(buffer + count) = (u_short)ePacketType::C_UpdateRoomListPage;		count += sizeof(u_short);
            *(char*)(buffer + count) = (char)curPage + 1;										count += sizeof(char);
            *(u_short*)buffer = count;
            NetworkManager::GetInst()->Send(buffer);
        });

    /* 룸 입장 불가 안내 팝업 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobby_joinroomfail.png");
    if (!pBitmap) return false;
    pPanel = new UIPanel(pBackground, 430, 640, ScreenWidth / 2, 0, 0.5f);
    pPanel->SetName(L"JoinRoomFail");
    pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddPopupUI(pPanel);
   
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_normal.png");
    if (!pBitmap) return false;
    pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() - 50, 0.5f, 1.f);
    pOKButton->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_pressed.png");
    if (!pBitmap) return false;
    pOKButton->SetPressedBitmap(pBitmap);
    pOKButton->SetCallback([]()
        {
            UIManager::GetInst()->PopPopupUI();
        });
    pOKButton->SetClickable(true);

    pPanel->AddChildUI(pOKButton);
    pPanel->SetActive(false);

    /* User List */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_userlist.png");
    if (!pBitmap) return false;
    UIPanel* pUserListPanel = new UIPanel(nullptr, 250, 400, ScreenWidth, 0, 1.0f, 0.f);
    pUserListPanel->SetName(L"UserListPanel");
    pUserListPanel->SetBitmap(pBitmap);
    UINT itemWidth = 200, itemHeight = 24;
    UIPage* pPage = new UIPage(pUserListPanel, itemWidth, 290, itemWidth, itemHeight, 20, 70, 0.f, 0.f, 8.f); // 페이지 당 리스트 아이템 수
    pPage->SetName(L"UserList");
    pUserListPanel->AddChildUI(pPage);
    UIManager::GetInst()->AddUI(pUserListPanel);
    pLayer->AddObj(pUserListPanel);

    UIPanel* pItem = new UIPanel(pPage, itemWidth, itemHeight);
    UIText* pNickname = new UIText(pItem, L"", 20.f);
    pNickname->SetName(L"Nickname");
    UIText* pSessionState = new UIText(pItem, L"", 10.f, 155, 5);
    pSessionState->SetName(L"SessionState");
    pItem->AddChildUI(pNickname);
    pItem->AddChildUI(pSessionState);
    pPage->SetItemTemplate(pItem);
    pPage->SetPrevBtnCallback([pPage]()
        {
            u_int curPage = pPage->GetCurPageIdx();
            if (curPage <= 0)
                return;

            char buffer[255];
            u_short count = sizeof(u_short);
            *(u_short*)(buffer + count) = (u_short)ePacketType::C_UpdateUserListPage;		count += sizeof(u_short);
            *(char*)(buffer + count) = (char)curPage - 1;										count += sizeof(char);
            *(u_short*)buffer = count;
            NetworkManager::GetInst()->Send(buffer);
        });
    pPage->SetNextBtnCallback([pPage]()
        {
            u_int curPage = pPage->GetCurPageIdx();

            char buffer[255];
            u_short count = sizeof(u_short);
            *(u_short*)(buffer + count) = (u_short)ePacketType::C_UpdateUserListPage;		count += sizeof(u_short);
            *(char*)(buffer + count) = (char)curPage + 1;										count += sizeof(char);
            *(u_short*)buffer = count;
            NetworkManager::GetInst()->Send(buffer);
        });
    delete pItem;   

    /* user profile */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_profile.png");
    if (!pBitmap) return false;
    UIPanel* pProfile = new UIPanel(nullptr, 500, 300, ScreenWidth, ScreenHeight, 1.0f, 1.0f);
    pProfile->SetBitmap(pBitmap);
    pProfile->SetName(L"Profile");
    UIText* pText = new UIText(pProfile, L"", 20.f, 220, 10);
    pText->SetName(L"ProfileText");
    pProfile->AddChildUI(pText);
    pText = new UIText(pProfile, L"0 킬", 20.f, 220, 40);
    pText->SetName(L"KillCountText");
    pProfile->AddChildUI(pText);
    UIManager::GetInst()->AddUI(pProfile);
    pLayer->AddObj(pProfile);

    /* Chat */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_chat.png");
    if (!pBitmap) return false;
    UIPanel* pChatPanel = new UIPanel(nullptr, 500, 300, 0, ScreenHeight, 0.f, 1.0f);
    pChatPanel->SetBitmap(pBitmap);
    pChatPanel->SetName(L"Chat");

    UIScrollView* pChatScrollView = new UIScrollView(pChatPanel, 480, 230, 10, 10, pChatPanel->GetWidth() - 80, 25, 2.f);
    pChatScrollView->SetName(L"ChatList");
    pChatPanel->AddChildUI(pChatScrollView);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pChatEdit = new UIEditText(pChatPanel, 385, 40, 17, 40, 285, 0.0f, 1.0f);
    pChatEdit->SetBitmap(pBitmap);
    pChatEdit->SetClickable(true);
    pChatEdit->SetCallback(&LobbyScene::ChatCallback, this);
    pChatPanel->AddChildUI(pChatEdit);
    UIManager::GetInst()->AddUI(pChatPanel);
    pLayer->AddObj(pChatPanel);
    
    return true;
}

bool LobbyScene::InitWaitingRoomUI()
{
    Layer* pLayer = CreateLayer(L"WaitingRoom_Background", 0, false);

    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene.png");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    pBackground->SetName(L"WaitingRoomBackground");
    pLayer->AddObj(pBackground);

    pLayer = CreateLayer(L"WaitingRoom_UI", 1, false);

    /* 게임 시작 버튼 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_start_normal.png");
    if (!pBitmap) return false;
    UIButton* pRoomBtn = new UIButton(nullptr, 150, 80, 970, 655, 0.5f, 0.f);
    pRoomBtn->SetName(L"StartGame");
    pRoomBtn->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_waiting_start_pressed.png");
    if (!pBitmap) return false;
    pRoomBtn->SetPressedBitmap(pBitmap);
    pRoomBtn->SetClickable(true);
    UIManager::GetInst()->AddUI(pRoomBtn);
    pLayer->AddObj(pRoomBtn);

    /* 유저 슬롯 01 */
    UIPanel* pUserPanel = new UIPanel(nullptr, 138, 174, 52, 160);
    pUserPanel->SetActive(false);
    pUserPanel->SetName(L"UserSlot0");
    UIManager::GetInst()->AddUI(pUserPanel);
    pLayer->AddObj(pUserPanel);
    UIPanel* pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    Bitmap* pRoomPlayerBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player0.png");
    if (!pRoomPlayerBitmap) return false;
    pUserPicture->SetBitmap(pRoomPlayerBitmap);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    UIText* pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth() / 2, 128, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pTextNickname->SetTextColor(D2D1::ColorF::White);
    pUserPanel->AddChildUI(pTextNickname);
    UIPanel* pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    Bitmap* pWaitBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (!pWaitBitmap) return false;
    pTextState->SetBitmap(pWaitBitmap);
    pTextState->SetName(L"State");
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 02 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 222, 160);
    pUserPanel->SetActive(false);
    pUserPanel->SetName(L"UserSlot1");
    UIManager::GetInst()->AddUI(pUserPanel);
    pLayer->AddObj(pUserPanel);
    pUserPicture->SetBitmap(pRoomPlayerBitmap);
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 128, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pTextNickname->SetTextColor(D2D1::ColorF::White);
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pTextState->SetBitmap(pWaitBitmap);
    pTextState->SetName(L"State");
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 03 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 392, 160);
    pUserPanel->SetActive(false);
    pUserPanel->SetName(L"UserSlot2");
    UIManager::GetInst()->AddUI(pUserPanel);
    pLayer->AddObj(pUserPanel);
    pUserPicture->SetBitmap(pRoomPlayerBitmap);
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 128, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pTextNickname->SetTextColor(D2D1::ColorF::White);
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pTextState->SetBitmap(pWaitBitmap);
    pTextState->SetName(L"State");
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 04 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 562, 160);
    pUserPanel->SetActive(false);
    pUserPanel->SetName(L"UserSlot3");
    UIManager::GetInst()->AddUI(pUserPanel);
    pLayer->AddObj(pUserPanel);
    pUserPicture->SetBitmap(pRoomPlayerBitmap);
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 128, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pTextNickname->SetTextColor(D2D1::ColorF::White);
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pTextState->SetBitmap(pWaitBitmap);
    pTextState->SetName(L"State");
    pUserPanel->AddChildUI(pTextState);

    /* 방에서 나가기 버튼 */
    UIButton* pBackBtn = new UIButton(nullptr, 47, 30, 1200, 764);
    pBackBtn->SetName(L"WaitingRoomBackBtn");
    pBackBtn->SetCallback([]() 
        {
            char buffer[255];
            ushort count = sizeof(ushort);
            *(ushort*)(buffer + count) = (ushort)ePacketType::C_LeaveRoom;		count += sizeof(ushort);
            *(ushort*)buffer = count;
            NetworkManager::GetInst()->Send(buffer);
        });
    pBackBtn->SetClickable(true);
    UIManager::GetInst()->AddUI(pBackBtn);
    pLayer->AddObj(pBackBtn);

    /* 게임 시작 불가 팝업 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waiting_startgamefail.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(pBackground, 430, 640, ScreenWidth / 2, 0, 0.5f);
    pPanel->SetName(L"StartGameFail");
    pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddPopupUI(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_normal.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() - 50, 0.5f, 1.f);
    pOKButton->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_pressed.png");
    if (!pBitmap) return false;
    pOKButton->SetPressedBitmap(pBitmap);
    pOKButton->SetCallback([]()
        {
            UIManager::GetInst()->PopPopupUI();
        });
    pOKButton->SetClickable(true);
    pPanel->AddChildUI(pOKButton);

    /* Chat */
    UIPanel* pChatPanel = new UIPanel(nullptr, 660, 320, 50, 410);
    pChatPanel->SetName(L"WaitingRoomChat");

    UIScrollView* pChatScrollView = new UIScrollView(pChatPanel, 660, 270, 0, 0, pChatPanel->GetWidth() - 80, 25, 2.f);
    pChatScrollView->SetName(L"WaitingRoomChatList");
    pChatPanel->AddChildUI(pChatScrollView);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pChatEdit = new UIEditText(pChatPanel, 350, 25, 16, 250, 315, 0.0f, 1.0f);
    pChatEdit->SetBitmap(pBitmap);
    pChatEdit->SetClickable(true);
    pChatEdit->SetCallback(&LobbyScene::ChatCallback, this);
    pChatPanel->AddChildUI(pChatEdit);
    UIManager::GetInst()->AddUI(pChatPanel);
    pLayer->AddObj(pChatPanel);

    /* Room Title */
    pPanel = new UIPanel(nullptr, 400, 30, 50, 80);
    pPanel->SetName(L"RoomTitle");
    UIText* pText = new UIText(pPanel, L"", 25);
    pText->SetName(L"Text");
    pPanel->AddChildUI(pText);
    UIManager::GetInst()->AddUI(pPanel);
    pLayer->AddObj(pPanel);

    /* UserChoice */
    pPanel = new UIPanel(nullptr, 440, 175, 788, 115);
    pPanel->SetName(L"PlayerChoicePanel");
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player0.png");
    if (!pBitmap) return false;
    UIButton* pBtn = new UIButton(pPanel, 92, 48);
    pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(0);
        });
    pPanel->AddChildUI(pBtn);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player1.png");
    if (!pBitmap) return false;
    pBtn = new UIButton(pPanel, 92, 48, 115);
    pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(1);
        });
    pPanel->AddChildUI(pBtn);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player2.png");
    if (!pBitmap) return false;
    pBtn = new UIButton(pPanel, 92, 48, 230);
    pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(2);
        });
    pPanel->AddChildUI(pBtn);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player3.png");
    if (!pBitmap) return false;
    pBtn = new UIButton(pPanel, 92, 48, 345);
    pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(3);
        });
    pPanel->AddChildUI(pBtn);
    UIManager::GetInst()->AddUI(pPanel);
    pLayer->AddObj(pPanel);

    return true;
}

void LobbyScene::GameStartCallback()
{
    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_CheckRoomReady;          count += sizeof(ushort);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void LobbyScene::GameReadyCallback()
{
    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_UserRoomReady;          count += sizeof(ushort);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void LobbyScene::Update()
{
    Scene::Update();

    // CheckNextState
    if (m_nextState == eSessionState::Lobby)
    {
        m_state = m_nextState;
        UpdateUserListPage();
        UpdateRoomListPage();

        ShowLobbyUI();
        HideWaitingRoomUI();
        m_nextState = eSessionState::None;
    }
    else if (m_nextState == eSessionState::WaitingRoom)
    {
        m_state = m_nextState;
        ShowWaitingRoomUI();
        HideLobbyUI();
        m_nextState = eSessionState::None;
    }

    if (m_state == eSessionState::Lobby)
    {
        m_frameTime -= Timer::GetInst()->GetDeltaTime();
        if (m_frameTime <= 0.f)
        {
            UpdateUserListPage();
            UpdateRoomListPage();

            m_frameTime = 5.f;
        }
    }
}

void LobbyScene::ChangeSceneUI(eSessionState _state)
{
    if (_state == eSessionState::Lobby || _state == eSessionState::WaitingRoom)
        m_nextState = _state;
}
