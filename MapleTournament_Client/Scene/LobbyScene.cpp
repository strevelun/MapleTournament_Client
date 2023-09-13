#include "../NetworkCore/NetworkManager.h"
#include "LobbyScene.h"
#include "../Managers/UIManager.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ObjectManager.h"
#include "../Bitmap.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIButton.h"
#include "../Obj/UI/UIEditText.h"
#include "../Obj/UI/UIList.h"
#include "../Obj/UI/UIText.h"
#include "../Obj/MyPlayer.h"
#include "../Setting.h"
#include "../Constants.h"

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
    if (_str.length() <= 0 || std::find(_str.begin(), _str.end(), L' ') != _str.end()) return;
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
    if (_str.length() <= 0 || std::find(_str.begin(), _str.end(), L' ') != _str.end()) return;
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

bool LobbyScene::ShowLobbyUI()
{
   

    return true;
}

bool LobbyScene::ShowWaitingRoomUI()
{
    UI* pUI = UIManager::GetInst()->FindUI(L"Background");
    if (!pUI) return false;
    pUI->SetActive(false);

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

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pRoomBtn = new UIButton(pBackground, 150, 100, pBackground->GetWidth() / 2, pBackground->GetHeight(), 0.5f, 1.0f);
    pRoomBtn->SetBitmap(pBitmap);
    UIText* pText = new UIText(pRoomBtn, L"방만들기",
        wcslen(L"방만들기") * 25, 25, pRoomBtn->GetWidth() / 2, 50, 0.5f, 0.7f);
    pRoomBtn->SetUIText(pText);
    pRoomBtn->SetCallback([]() {
        UIPanel* pCreateRoom = static_cast<UIPanel*>(UIManager::GetInst()->FindUI(L"CreateRoom"));
        pCreateRoom->SetActive(true);
        UIManager::GetInst()->AddPopupUI(pCreateRoom);
        });
    pRoomBtn->SetClickable(true);
    pBackground->AddChildUI(pRoomBtn);

    /* 방만들기 팝업 */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(pBackground, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
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
    pText = new UIText(pOKButton, L"만들기!",
        pOKButton->GetWidth(), 25, pOKButton->GetWidth() / 2, 50, 0.5f, 0.5f);
    pOKButton->SetUIText(pText);

    pPanel->AddChildUI(pOKButton);
    pOKButton->SetClickable(true);
    pPanel->SetActive(false);

    /* Room List */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_roomlist.png");
    if (!pBitmap) return false;
    UIPanel* pRoomListPanel = new UIPanel(pBackground, 800, 400);
    pRoomListPanel->SetName(L"RoomListPanel");
    pRoomListPanel->SetBitmap(pBitmap);
    UIList* pRoomList = new UIList(pRoomListPanel, 680, 355, 20, 50);
    pRoomList->SetName(L"RoomList");
    pRoomListPanel->AddChildUI(pRoomList);
    pUIManager->AddUI(pRoomListPanel);
    pBackground->AddChildUI(pRoomListPanel);

    MyPlayer* pMyPlayer = ObjectManager::GetInst()->GetMyPlayer();
    const std::wstring& myNickname = pMyPlayer->GetNickname();
    const wchar_t* pMyNickname = myNickname.c_str();

    /* User List */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_userlist.png");
    if (!pBitmap) return false;
    UIPanel* pUserListPanel = new UIPanel(pBackground, 250, 400, ScreenWidth, 0, 1.0f, 0.f);
    pUserListPanel->SetName(L"UserListPanel");
    pUserListPanel->SetBitmap(pBitmap);
    UIList* pUserList = new UIList(pUserListPanel, 250, 330, 10, 70, 0.f, 0.f);
    pUserList->SetName(L"UserList");
    pText = new UIText(pUserList, pMyNickname, wcslen(pMyNickname) * 25, 25);
    pUserList->AddItem(pText);
    pUserListPanel->AddChildUI(pUserList);
    pUIManager->AddUI(pUserListPanel);
    pBackground->AddChildUI(pUserListPanel);

    /* user profile */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_profile.png");
    if (!pBitmap) return false;
    UIPanel* pProfile = new UIPanel(pBackground, 400, 200, ScreenWidth, ScreenHeight, 1.0f, 1.0f);
    pProfile->SetBitmap(pBitmap);
    pProfile->SetName(L"Profile");
    pText = new UIText(pProfile, pMyNickname, wcslen(pMyNickname) * 25, 25, pProfile->GetWidth() / 2, 25, 0.5f, 0.5f);
    pProfile->AddChildUI(pText);
    pUIManager->AddUI(pProfile);
    pBackground->AddChildUI(pProfile);

    /* Chat */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_chat.png");
    if (!pBitmap) return false;
    UIPanel* pChatPanel = new UIPanel(pBackground, 500, 300, 0, ScreenHeight, 0.f, 1.0f);
    pChatPanel->SetBitmap(pBitmap);
    pChatPanel->SetName(L"Chat");
    pUIManager->AddUI(pChatPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_lobbyscene_roombutton.png");
    if (!pBitmap) return false;
    UIList* pChatList = new UIList(pChatPanel, 480, 230, 10, 10, 0.f, 0.f);
    pChatList->SetBitmap(pBitmap);
    pChatList->SetName(L"ChatList");
    pChatPanel->AddChildUI(pChatList);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pChatEdit = new UIEditText(pChatPanel, 385, 40, 40, 285, 0.0f, 1.0f);
    pChatEdit->SetBitmap(pBitmap);
    pChatEdit->SetClickable(true);
    pChatEdit->SetCallback(&LobbyScene::ChatCallback, this);
    pChatPanel->AddChildUI(pChatEdit);
    pBackground->AddChildUI(pChatPanel);
    
    return true;
}

bool LobbyScene::InitWaitingRoomUI()
{
    return true;
}
