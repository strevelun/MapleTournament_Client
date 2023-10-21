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
#include "../Obj/MyPlayer.h"
#include "../Setting.h"
#include "../Constants.h"

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

    // 
    ShowLobbyUI();

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_EnterLobby;		count += sizeof(ushort);
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
    /*
    UI* pUI = UIManager::GetInst()->FindUI(L"Chat");
    UIPanel* pPanel = static_cast<UIPanel*>(pUI);
    pUI = pPanel->FindChildUI(L"ChatList");
    UIScrollView* pScroll = static_cast<UIScrollView*>(pUI);
    size_t ct = pScroll->GetUIList()->GetItemCount();
    pScroll->SetIdx(pScroll->GetTopIdx() + pScroll->GetBottomIdx() - ct, ct + 1);
    */
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
    UIEditText* pInputRoomName = new UIEditText(pPanel, pPanel->GetWidth() - 100, 50, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2, 0.5f, 0.5f);
    pInputRoomName->SetBitmap(pBitmap);
    pInputRoomName->SetClickable(true);
    pInputRoomName->SetCallback(&LobbyScene::CreateRoomButtonCallback, this);
    pPanel->AddChildUI(pInputRoomName);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 0.5f, 0.5f);
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

    /* Room List */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_roomlist.png");
    if (!pBitmap) return false;
    UIPanel* pRoomListPanel = new UIPanel(nullptr, 800, 400);
    pRoomListPanel->SetName(L"RoomListPanel");
    pRoomListPanel->SetBitmap(pBitmap);
    UIList* pRoomList = new UIList(pRoomListPanel, 680, 355, 665, 20, 50);
    pRoomList->SetName(L"RoomList");
    pRoomListPanel->AddChildUI(pRoomList);
    pUIManager->AddUI(pRoomListPanel);
    //pBackground->AddChildUI(pRoomListPanel);

    MyPlayer* pMyPlayer = ObjectManager::GetInst()->GetMyPlayer();
    const std::wstring& myNickname = pMyPlayer->GetNickname();
    const wchar_t* pMyNickname = myNickname.c_str();

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
    UIList* pUserList = new UIList(pUserListPanel, 250, 330, 110, 24, 10, 70, 0.f, 0.f);
    pUserList->SetName(L"UserList");
    pUserListPanel->AddChildUI(pUserList);
    pUserList->SetItemVerticalInterval(9.0f);
    pUIManager->AddUI(pUserListPanel);

    /* user profile */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_profile.png");
    if (!pBitmap) return false;
    UIPanel* pProfile = new UIPanel(nullptr, 400, 200, ScreenWidth, ScreenHeight, 1.0f, 1.0f);
    pProfile->SetBitmap(pBitmap);
    pProfile->SetName(L"Profile");
    pText = new UIText(pProfile, pMyNickname, 20.f, pProfile->GetWidth() / 2, 10);
    pProfile->AddChildUI(pText);
    pUIManager->AddUI(pProfile);
    //pBackground->AddChildUI(pProfile);

    /* Chat */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_chat.png");
    if (!pBitmap) return false;
    UIPanel* pChatPanel = new UIPanel(nullptr, 500, 300, 0, ScreenHeight, 0.f, 1.0f);
    pChatPanel->SetBitmap(pBitmap);
    pChatPanel->SetName(L"Chat");

    UIScrollView* pChatScrollView = new UIScrollView(pChatPanel, 480, 230, 10, 10, 665, 20);
    pChatScrollView->SetName(L"ChatList");
    pChatScrollView->GetUIList()->SetItemVerticalInterval(2.f);
    pChatPanel->AddChildUI(pChatScrollView);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pChatEdit = new UIEditText(pChatPanel, 385, 40, 40, 285, 0.0f, 1.0f);
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
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\profile_character.png");
    if (!pBitmap) return false;
    UIPanel* pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    UIText* pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth() / 2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    UIPanel* pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (!pBitmap) return false;
    pTextState->SetName(L"State");
    pTextState->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 02 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 222, 160);
    pUserPanel->SetName(L"UserSlot1");
    pUserPanel->SetActive(false);
    UIManager::GetInst()->AddUI(pUserPanel);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\profile_character.png");
    if (!pBitmap) return false;
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (!pBitmap) return false;
    pTextState->SetName(L"State");
    pTextState->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 03 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 392, 160);
    pUserPanel->SetName(L"UserSlot2");
    pUserPanel->SetActive(false);
    UIManager::GetInst()->AddUI(pUserPanel);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\profile_character.png");
    if (!pBitmap) return false;
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (!pBitmap) return false;
    pTextState->SetName(L"State");
    pTextState->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pTextState);

    /* 유저 슬롯 04 */
    pUserPanel = new UIPanel(nullptr, 138, 174, 562, 160);
    pUserPanel->SetName(L"UserSlot3");
    pUserPanel->SetActive(false);
    UIManager::GetInst()->AddUI(pUserPanel);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\profile_character.png");
    if (!pBitmap) return false;
    pUserPicture = new UIPanel(pUserPanel, 138, 113, 0, 0);
    pUserPicture->SetBitmap(pBitmap);
    pUserPanel->AddChildUI(pUserPicture);
    pTextNickname = new UIText(pUserPanel, L"", 20.f, pUserPanel->GetWidth()/2, 132, 0.5f, 0.f);
    pTextNickname->SetName(L"Nickname");
    pUserPanel->AddChildUI(pTextNickname);
    pTextState = new UIPanel(pUserPanel, pUserPanel->GetWidth(), 20, pUserPanel->GetWidth() / 2, 155, 0.5f, 0.f);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_waitingroomscene_wait.png");
    if (!pBitmap) return false;
    pTextState->SetName(L"State");
    pTextState->SetBitmap(pBitmap);
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

            count = sizeof(ushort);
            *(ushort*)(buffer + count) = (ushort)ePacketType::C_EnterLobby;		count += sizeof(ushort);
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