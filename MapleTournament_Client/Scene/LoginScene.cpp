#include "../NetworkCore/NetworkManager.h"
#include "LoginScene.h"
#include "Layer.h"
#include "../Obj/UI/UIPanel.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/UIManager.h"
#include "../Managers/SceneManager.h"
#include "../Obj/UI/UIEditText.h"
#include "../Obj/UI/UIButton.h"
#include "../Obj/UI/UIText.h"
#include "../Setting.h"
#include "../Constants.h"

#include <string>
#include <algorithm>

typedef unsigned short ushort;

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

// 로그인 
// - 성공 (닉네임 존재 안할때 유저 등록 후 자동 로그인, 이미 등록된 닉네임일때)
// - 실패 (양식에 어긋남 or 누가 해당 닉네임으로 로그인 중 or 
bool LoginScene::Init()
{
    Layer* pLayer = CreateLayer(L"Background", 0);
    
    /* Background */;
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_loginscene_background.png");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    pLayer->AddObj(pBackground);

    pLayer = CreateLayer(L"UI", 1);

    /* UI */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pInputNickname = new UIEditText(pBackground, 500, 50, 13, pBackground->GetWidth() / 2, pBackground->GetHeight() / 2, 0.5f, 0.5f);
    pInputNickname->SetBitmap(pBitmap);
    pInputNickname->SetClickable(true);
    pInputNickname->SetCallback(&LoginScene::LoginButtonCallback, this);
    pBackground->AddChildUI(pInputNickname);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pLoginButton = new UIButton(pBackground, 100, 50,
         (pInputNickname->GetPosX()) + pInputNickname->GetWidth() + 10.f, pInputNickname->GetPosY(), 0.f, 0.f);
    pLoginButton->SetBitmap(pBitmap);
    UIText* pText = new UIText(pLoginButton, L"로그인", 20.f, pLoginButton->GetWidth() / 2, pLoginButton->GetHeight() / 2, 0.5f, 0.7f);
    pLoginButton->SetUIText(pText);
    pLoginButton->SetClickable(true);
    pLoginButton->SetCallback([this, pInputNickname]() { LoginButtonCallback(pInputNickname, pInputNickname->GetText()); });
    pBackground->AddChildUI(pLoginButton);

    /* PopupUI 로그인 실패(잘못입력) */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(nullptr, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    pPanel->SetBitmap(pBitmap);
    pPanel->SetName(L"WrongInput");
    pPanel->SetActive(false);
    pText = new UIText(pPanel, L"잘못된 입력입니다.", 20.f, pPanel->GetWidth() / 2, 100, 0.5f, 0.5f);
    pPanel->AddChildUI(pText);
    UIManager::GetInst()->AddPopupUI(pPanel);
    //pLayer->AddObj(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 0.5f, 0.5f);
    pOKButton->SetBitmap(pBitmap);
    pOKButton->SetCallback([]() { UIManager::GetInst()->PopPopupUI(); });
    pText = new UIText(pOKButton, L"확인", 20.f, pOKButton->GetWidth() / 2, pOKButton->GetHeight() / 2, 0.5f, 0.7f);
    pOKButton->SetUIText(pText);
    pPanel->AddChildUI(pOKButton);
    pOKButton->SetClickable(true);


    /* PopupUI 로그인 실패(이미로그인 중) */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    if (!pBitmap) return false;
    pPanel = new UIPanel(nullptr, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    pPanel->SetBitmap(pBitmap);
    pPanel->SetName(L"AlreadyLogin");
    pPanel->SetActive(false);
    pText = new UIText(pPanel, L"누군가 이 닉네임으로 로그인 중", 20.f, pPanel->GetWidth() / 2, 100, 0.5f, 0.5f);
    pPanel->AddChildUI(pText);
    UIManager::GetInst()->AddPopupUI(pPanel);
    //pLayer->AddObj(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 0.5f, 0.5f);
    pOKButton->SetBitmap(pBitmap);
    pOKButton->SetCallback([]() { UIManager::GetInst()->PopPopupUI(); });
    pText = new UIText(pOKButton, L"확인", 20.f, pOKButton->GetWidth()/2, pOKButton->GetHeight()/2, 0.5f, 0.7f);
    pOKButton->SetUIText(pText);
    pPanel->AddChildUI(pOKButton);
    pOKButton->SetClickable(true);

    return true;
}

void LoginScene::LoginButtonCallback(UIEditText* _pEditText, const std::wstring& _str)
{
    if (_str.length() <= 0 || std::all_of(_str.begin(), _str.end(), [](wchar_t c) { return c == L' '; }))
    {
        UI* pUI = UIManager::GetInst()->FindUI(L"WrongInput");
        if (pUI) UIManager::GetInst()->SetPopupUI(static_cast<UIPanel*>(pUI));
        return;
    }
    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_OKLogin;						count += sizeof(ushort);
    const wchar_t* str = _str.c_str();
    memcpy(buffer + count, str, wcslen(str) * 2);				count += (ushort)wcslen(str) * 2;
    *(wchar_t*)(buffer + count) = L'\0';									count += 2;
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void LoginScene::EndButtonCallback()
{
}
