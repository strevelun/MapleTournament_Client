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
#include <cwctype>

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
    UIEditText* pInputNickname = new UIEditText(pBackground, 500, 50, 7, (float)pBackground->GetWidth() / 2, (float)pBackground->GetHeight() / 2, 0.5f, 0.5f);
    pInputNickname->SetBitmap(pBitmap);
    pInputNickname->SetClickable(true);
    pInputNickname->SetCallback(&LoginScene::LoginButtonCallback, this);
    pBackground->AddChildUI(pInputNickname);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_login_normal.png");
    if (!pBitmap) return false;
    UIButton* pLoginButton = new UIButton(pBackground, 100, 50,
         (pInputNickname->GetPosX()) + pInputNickname->GetWidth() + 10.f, pInputNickname->GetPosY(), 0.f, 0.f);
    pLoginButton->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_login_pressed.png");
    if (!pBitmap) return false;
    pLoginButton->SetPressedBitmap(pBitmap);
    pLoginButton->SetClickable(true);
    pLoginButton->SetCallback([this, pInputNickname]() { LoginButtonCallback(pInputNickname, pInputNickname->GetText()); });
    pBackground->AddChildUI(pLoginButton);

    /* PopupUI 로그인 실패(잘못입력) */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_wronginput.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(nullptr, 430, 640, ScreenWidth / 2, 0, 0.5f);
    pPanel->SetBitmap(pBitmap);
    pPanel->SetName(L"WrongInput");
    pPanel->SetActive(false);
    UIManager::GetInst()->AddPopupUI(pPanel);
    //pLayer->AddObj(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_normal.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(pPanel, 150, 100, pPanel->GetWidth() / 2.f, pPanel->GetHeight() - 50.f, 0.5f, 1.f);
    pOKButton->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_pressed.png");
    if (!pBitmap) return false;
    pOKButton->SetPressedBitmap(pBitmap);
    pOKButton->SetCallback([]() { UIManager::GetInst()->PopPopupUI(); });
    pPanel->AddChildUI(pOKButton);
    pOKButton->SetClickable(true);


    /* PopupUI 로그인 실패(이미로그인 중) */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_login_alreadylogin.png");
    if (!pBitmap) return false;
    pPanel = new UIPanel(nullptr, 430, 640, ScreenWidth / 2, 0, 0.5f);
    pPanel->SetBitmap(pBitmap);
    pPanel->SetName(L"AlreadyLogin");
    pPanel->SetActive(false);
    UIManager::GetInst()->AddPopupUI(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_normal.png");
    if (!pBitmap) return false;
    pOKButton = new UIButton(pPanel, 150, 100, (float)pPanel->GetWidth() / 2, (float)pPanel->GetHeight() - 50, 0.5f, 1.f);
    pOKButton->SetBitmap(pBitmap);
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button_okay_pressed.png");
    if (!pBitmap) return false;
    pOKButton->SetPressedBitmap(pBitmap);
    pOKButton->SetCallback([]() { UIManager::GetInst()->PopPopupUI(); });
    pPanel->AddChildUI(pOKButton);
    pOKButton->SetClickable(true);

    return true;
}

void LoginScene::LoginButtonCallback(UIEditText* _pEditText, const std::wstring& _str)
{
    if (_str.empty() || std::any_of(_str.begin(), _str.end(), [](wchar_t c) { return std::iswspace(c); }))
    {
        UI* pUI = UIManager::GetInst()->FindUI(L"WrongInput");
        if (pUI) UIManager::GetInst()->SetPopupUI(static_cast<UIPanel*>(pUI));
        return;
    }
    char buffer[255] = {};
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_OKLogin;						count += sizeof(ushort);
    const wchar_t* str = _str.c_str();
    int len = (int)wcslen(str) * 2;
    memcpy(buffer + count, str, len);			                            count += len;
    *(wchar_t*)(buffer + count) = L'\0';									count += 2;
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void LoginScene::EndButtonCallback()
{
}
