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

    _pEditText->GetParent()->SetActive(false); 
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
    UI* pUI = UIManager::GetInst()->FindUI(L"Background");
    if (!pUI) return false;
    pUI->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"MakeRoom");
    if (!pUI) return false;
    pUI->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
    if (!pUI) return false;
    pUI->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
    if (!pUI) return false;
    pUI->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"Profile");
    if (!pUI) return false;
    pUI->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"Chat");
    if (!pUI) return false;
    pUI->SetActive(true);

    return true;
}

bool LobbyScene::ShowWaitingRoomUI()
{
    /* UI 활성화 */
    UI* pUI = UIManager::GetInst()->FindUI(L"WaitingRoomBackground");
    if (!pUI) return false;
    pUI->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"StartGame");
    if (!pUI) return false;
    UIButton* pBtn = static_cast<UIButton*>(pUI);
    pBtn->SetActive(true);
  
    pUI = UIManager::GetInst()->FindUI(L"WaitingRoomBackBtn");
    if (!pUI) return false;
    UIButton* pBackBtn = static_cast<UIButton*>(pUI);
    pBackBtn->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"WaitingRoomChat");
    if (!pUI) return false;
    pBtn = static_cast<UIButton*>(pUI);
    pBtn->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"RoomTitle");
    if (!pUI) return false;
    UIPanel* pText = static_cast<UIPanel*>(pUI);
    pText->SetActive(true);

    pUI = UIManager::GetInst()->FindUI(L"PlayerChoicePanel");
    if (!pUI) return false;
    UIPanel* pPanel = static_cast<UIPanel*>(pUI);
    pPanel->SetActive(true);
    
    return true;
}

bool LobbyScene::HideLobbyUI()
{
    UI* pUI = UIManager::GetInst()->FindUI(L"Background");
    if (!pUI) return false;
    pUI->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"MakeRoom");
    if (!pUI) return false;
    pUI->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"RoomListPanel");
    if (!pUI) return false;
    pUI->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"UserListPanel");
    if (!pUI) return false;
    pUI->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"Profile");
    if (!pUI) return false;
    pUI->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"Chat");
    if (!pUI) return false;
    pUI->SetActive(false);

    return true;
}

bool LobbyScene::HideWaitingRoomUI()
{
    UI* pUI = UIManager::GetInst()->FindUI(L"WaitingRoomBackground");
    if (!pUI) return false;
    pUI->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"StartGame");
    if (!pUI) return false;
    UIButton* pBtn = static_cast<UIButton*>(pUI);
    pBtn->SetActive(false);
    
    pUI = UIManager::GetInst()->FindUI(L"UserSlot0");
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
    
    pUI = UIManager::GetInst()->FindUI(L"WaitingRoomBackBtn");
    if (!pUI) return false;
    UIButton* pBackBtn = static_cast<UIButton*>(pUI);
    pBackBtn->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"WaitingRoomChat");
    if (!pUI) return false;
    pBtn = static_cast<UIButton*>(pUI);
    pBtn->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"RoomTitle");
    if (!pUI) return false;
    UIPanel* pText = static_cast<UIPanel*>(pUI);
    pText->SetActive(false);

    pUI = UIManager::GetInst()->FindUI(L"PlayerChoicePanel");
    if (!pUI) return false;
    UIPanel* pPanel = static_cast<UIPanel*>(pUI);
    pPanel->SetActive(false);
    
    return true;
}

bool LobbyScene::InitLobbyUI()
{
    UIManager* pUIManager = UIManager::GetInst();
    /* Background */;
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_background2.png");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    pBackground->SetName(L"Background");
    pUIManager->AddUI(pBackground);

    /* 방만들기 버튼 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pRoomBtn = new UIButton(nullptr, 150, 100, pBackground->GetWidth() / 2, pBackground->GetHeight(), 0.5f, 1.0f);
    pRoomBtn->SetName(L"MakeRoom");
    pRoomBtn->SetBitmap(pBitmap);
    UIText* pText = new UIText(pRoomBtn, L"방만들기", 20.f, pRoomBtn->GetWidth() / 2, 50, 0.5f, 0.7f);
    pRoomBtn->SetUIText(pText);
    pRoomBtn->SetCallback([]() {
        UIPanel* pCreateRoom = static_cast<UIPanel*>(UIManager::GetInst()->FindUI(L"CreateRoom"));
        pCreateRoom->SetActive(true);
        UIManager::GetInst()->AddPopupUI(pCreateRoom);
        });
    pRoomBtn->SetClickable(true);
    pUIManager->AddUI(pRoomBtn);

    /* 방만들기 팝업 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(nullptr, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    pPanel->SetName(L"CreateRoom");
    pPanel->SetBitmap(pBitmap);
    pUIManager->AddUI(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pInputRoomName = new UIEditText(pPanel, pPanel->GetWidth() - 100, 50, 20, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2, 0.5f, 0.5f);
    pInputRoomName->SetBitmap(pBitmap);
    pInputRoomName->SetClickable(true);
    pInputRoomName->SetCallback(&LobbyScene::CreateRoomButtonCallback, this);
    pPanel->AddChildUI(pInputRoomName);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2 - 10, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 1.0f, 0.5f);
    pOKButton->SetBitmap(pBitmap);
    pOKButton->SetCallback([this, pPanel, pInputRoomName]()
        {
            CreateRoomButtonCallback(pInputRoomName, pInputRoomName->GetText());
        });
    pText = new UIText(pOKButton, L"만들기!", 20.f, pOKButton->GetWidth() / 2, 50, 0.5f, 0.5f);
    pOKButton->SetUIText(pText);
    pOKButton->SetClickable(true);
    pPanel->AddChildUI(pOKButton);
    pPanel->SetActive(false);


    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pCancelButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2 + 10, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 0.f, 0.5f);
    pCancelButton->SetBitmap(pBitmap);
    pCancelButton->SetCallback([this, pPanel]()
        {
            pPanel->SetActive(false);
            UIManager::GetInst()->PopPopupUI();
        });
    pText = new UIText(pCancelButton, L"취소", 20.f, pCancelButton->GetWidth() / 2, 50, 0.5f, 0.5f);
    pCancelButton->SetUIText(pText);
    pCancelButton->SetClickable(true);
    pPanel->AddChildUI(pCancelButton);
    pPanel->SetActive(false);

    /* Room List */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_roomlist.png");
    if (!pBitmap) return false;
    UIPanel* pRoomListPanel = new UIPanel(nullptr, 800, 400);
    pRoomListPanel->SetName(L"RoomListPanel");
    pRoomListPanel->SetBitmap(pBitmap);
    UIPage* pRoomPage = new UIPage(pRoomListPanel, 645, 310, 645, 25, 80, 45, 0.f, 0.f, 6.f);
    pRoomPage->SetName(L"RoomList");
    pRoomListPanel->AddChildUI(pRoomPage);
    pUIManager->AddUI(pRoomListPanel);
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
    UIText* pTextOwner = new UIText(pPanel, L"", 20.f, 460.f, 0.f);
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
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    if (!pBitmap) return false;
    pPanel = new UIPanel(pBackground, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    pPanel->SetName(L"JoinRoomFail");
    pPanel->SetBitmap(pBitmap);
    pUIManager->AddUI(pPanel);
    pText = new UIText(pPanel, L"방이 꽉 차거나 게임중입니다!", 20.f, pPanel->GetWidth() / 2, 100, 0.5f, 0.5f);
    pPanel->AddChildUI(pText);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 0.5f, 0.5f);
    pOKButton->SetBitmap(pBitmap);
    pOKButton->SetCallback([this, pPanel]()
        {
            pPanel->SetActive(false);
            UIManager::GetInst()->PopPopupUI();
        });
    pText = new UIText(pOKButton, L"넹", 20.f, pOKButton->GetWidth() / 2, 50, 0.5f, 0.5f);
    pOKButton->SetUIText(pText);
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
    UIPage* pPage = new UIPage(pUserListPanel, itemWidth, 290, itemWidth, itemHeight, 10, 70, 0.f, 0.f, 8.f); // 페이지 당 리스트 아이템 수
    pPage->SetName(L"UserList");
    pUserListPanel->AddChildUI(pPage);
    pUIManager->AddUI(pUserListPanel);
    UIPanel* pItem = new UIPanel(pPage, itemWidth, itemHeight);
    UIText* pNickname = new UIText(pItem, L"", 20.f);
    pNickname->SetName(L"Nickname");
    UIText* pSessionState = new UIText(pItem, L"", 10.f, 160);
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
    UIPanel* pProfile = new UIPanel(nullptr, 400, 200, ScreenWidth, ScreenHeight, 1.0f, 1.0f);
    pProfile->SetBitmap(pBitmap);
    pProfile->SetName(L"Profile");
    pText = new UIText(pProfile, L"", 20.f, pProfile->GetWidth() / 2, 50);
    pText->SetName(L"ProfileText");
    pProfile->AddChildUI(pText);
    pText = new UIText(pProfile, L"맞은 횟수 : 0", 20.f, pProfile->GetWidth() / 2 + 20, 120);
    pText->SetName(L"HitCountText");
    pProfile->AddChildUI(pText);
    pUIManager->AddUI(pProfile);
    //pBackground->AddChildUI(pProfile);

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
    UIEditText* pChatEdit = new UIEditText(pChatPanel, 385, 40, 20, 40, 285, 0.0f, 1.0f);
    pChatEdit->SetBitmap(pBitmap);
    pChatEdit->SetClickable(true);
    pChatEdit->SetCallback(&LobbyScene::ChatCallback, this);
    pChatPanel->AddChildUI(pChatEdit);
    pUIManager->AddUI(pChatPanel);
    
    return true;
}

bool LobbyScene::InitWaitingRoomUI()
{
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene.png");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    pBackground->SetName(L"WaitingRoomBackground");
    pBackground->SetActive(false);
    UIManager::GetInst()->AddUI(pBackground);

    /* 게임 시작 버튼 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pRoomBtn = new UIButton(nullptr, 150, 80, 970, 655, 0.5f, 0.f);
    pRoomBtn->SetName(L"StartGame");
    pRoomBtn->SetBitmap(pBitmap);
    UIText* pText = new UIText(pRoomBtn, L"게임 시작", 20.f, pRoomBtn->GetWidth() / 2, pRoomBtn->GetHeight() / 2, 0.4f, 0.7f);
    pRoomBtn->SetUIText(pText);
    pRoomBtn->SetClickable(true);
    pRoomBtn->SetActive(false);
    UIManager::GetInst()->AddUI(pRoomBtn);

    /* 유저 슬롯 01 */
    UIPanel* pUserPanel = new UIPanel(nullptr, 138, 174, 52, 160);
    pUserPanel->SetName(L"UserSlot0");
    pUserPanel->SetActive(false);
    UIManager::GetInst()->AddUI(pUserPanel);
    UIPanel* pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player0.png");
    if (pBitmap)         pUserPicture->SetBitmap(pBitmap);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    UIText* pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth() / 2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    UIPanel* pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (pBitmap)  pTextState->SetBitmap(pBitmap);
    pTextState->SetName(L"State");
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 02 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 222, 160);
    pUserPanel->SetName(L"UserSlot1");
    pUserPanel->SetActive(false);
    UIManager::GetInst()->AddUI(pUserPanel);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player0.png");
    if (pBitmap)         pUserPicture->SetBitmap(pBitmap);
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (pBitmap)  pTextState->SetBitmap(pBitmap);
    pTextState->SetName(L"State");
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 03 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 392, 160);
    pUserPanel->SetName(L"UserSlot2");
    pUserPanel->SetActive(false);
    UIManager::GetInst()->AddUI(pUserPanel);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player0.png");
    if (pBitmap)         pUserPicture->SetBitmap(pBitmap);
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (pBitmap)  pTextState->SetBitmap(pBitmap);
    pTextState->SetName(L"State");
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 04 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 562, 160);
    pUserPanel->SetName(L"UserSlot3");
    pUserPanel->SetActive(false);
    UIManager::GetInst()->AddUI(pUserPanel);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_room_player0.png");
    if (pBitmap)         pUserPicture->SetBitmap(pBitmap);
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetName(L"Picture");
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (pBitmap)  pTextState->SetBitmap(pBitmap);
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
    pBackBtn->SetActive(false);
    UIManager::GetInst()->AddUI(pBackBtn);

    /* 게임 시작 불가 팝업 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(pBackground, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    pPanel->SetName(L"StartGameFail");
    pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddUI(pPanel);
    pText = new UIText(pPanel, L"전부 레디하지 않았음!", 20.f, pPanel->GetWidth() / 2, 100, 0.5f, 0.5f);
    pPanel->AddChildUI(pText);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 0.5f, 0.5f);
    pOKButton->SetBitmap(pBitmap);
    pOKButton->SetCallback([this, pPanel]()
        {
            pPanel->SetActive(false);
            UIManager::GetInst()->PopPopupUI();
        });
    pText = new UIText(pOKButton, L"네", 20.f, pOKButton->GetWidth() / 2, 50, 0.5f, 0.5f);
    pOKButton->SetUIText(pText);
    pOKButton->SetClickable(true);

    pPanel->AddChildUI(pOKButton);
    pPanel->SetActive(false);

    /* Chat */
    UIPanel* pChatPanel = new UIPanel(nullptr, 660, 320, 50, 410);
    pChatPanel->SetName(L"WaitingRoomChat");

    UIScrollView* pChatScrollView = new UIScrollView(pChatPanel, 660, 270, 0, 0, pChatPanel->GetWidth() - 80, 25, 2.f);
    pChatScrollView->SetName(L"WaitingRoomChatList");
    pChatPanel->AddChildUI(pChatScrollView);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pChatEdit = new UIEditText(pChatPanel, 350, 25, 20, 250, 315, 0.0f, 1.0f);
    pChatEdit->SetBitmap(pBitmap);
    pChatEdit->SetClickable(true);
    pChatEdit->SetCallback(&LobbyScene::ChatCallback, this);
    pChatPanel->AddChildUI(pChatEdit);
    UIManager::GetInst()->AddUI(pChatPanel);
    pChatPanel->SetActive(false);

    /* Room Title */
    pPanel = new UIPanel(nullptr, 200, 30, 50, 80);
    pPanel->SetName(L"RoomTitle");
    pPanel->SetActive(false);
    pText = new UIText(pPanel, L"", 25);
    pText->SetName(L"Text");
    pPanel->AddChildUI(pText);
    UIManager::GetInst()->AddUI(pPanel);

    /* UserChoice */
    pPanel = new UIPanel(nullptr, 440, 175, 788, 115);
    pPanel->SetName(L"PlayerChoicePanel");
    pPanel->SetActive(false);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player0.png");
    UIButton* pBtn = new UIButton(pPanel, 92, 48);
    if (pBitmap) pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(0);
        });
    pPanel->AddChildUI(pBtn);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player1.png");
    pBtn = new UIButton(pPanel, 92, 48, 115);
    if (pBitmap) pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(1);
        });
    pPanel->AddChildUI(pBtn);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player2.png");
    pBtn = new UIButton(pPanel, 92, 48, 230);
    if (pBitmap) pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(2);
        });
    pPanel->AddChildUI(pBtn);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_player3.png");
    pBtn = new UIButton(pPanel, 92, 48, 345);
    if (pBitmap) pBtn->SetBitmap(pBitmap);
    pBtn->SetClickable(true);
    pBtn->SetCallback([this]()
        {
            UpdateUserSlot(3);
        });
    pPanel->AddChildUI(pBtn);
    UIManager::GetInst()->AddUI(pPanel);

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
    if (_state == eSessionState::Lobby)
    {
        UpdateUserListPage();
        UpdateRoomListPage();

        ShowLobbyUI();
        HideWaitingRoomUI();
        m_state = _state;
    }
    else if (_state == eSessionState::WaitingRoom)
    {
        ShowWaitingRoomUI();
        HideLobbyUI();
        m_state = _state;
    }
}
