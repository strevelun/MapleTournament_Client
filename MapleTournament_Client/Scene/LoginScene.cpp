#include "../NetworkCore/NetworkManager.h"
#include "LoginScene.h"
#include "Layer.h"
#include "../Obj/UI/UIPanel.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/UIManager.h"
#include "../Managers/SceneManager.h"
#include "../Obj/UI/UIEditText.h"
#include "../Obj/UI/UIButton.h"
#include "../Packet/EnterPacket.h"

#include <string>

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

bool LoginScene::Init()
{
    UIManager* pUIManager = UIManager::GetInst();
    /* Background */;
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_loginscene_background.png");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(1920, 1080);
    pBackground->SetBitmap(pBitmap);
    pUIManager->AddUI(pBackground);

    /* UI */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_loginscene_login_edittext.png");
    if (!pBitmap) return false;
    UIEditText* pInputNickname = new UIEditText(500, 50, pBackground->GetWidth() / 2, pBackground->GetHeight() / 2, 0.5f, 0.5f);
    pInputNickname->SetBitmap(pBitmap);
    pInputNickname->SetClickable(true);
    pInputNickname->SetCallback(&LoginScene::LoginButtonCallback, this);
    pBackground->AddChildUI(pInputNickname);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pLoginButton = new UIButton(100, 50,
        pBackground->GetWidth() / 2 + (pInputNickname->GetWidth() / 2) + 10.f, pInputNickname->GetPosY(), 0.f, 0.5f);
    pLoginButton->SetBitmap(pBitmap);
    pLoginButton->SetText(L"로그인");
    pLoginButton->SetClickable(true);
    pLoginButton->SetCallback([this, pInputNickname]() { LoginButtonCallback(pInputNickname->GetText()); });
    pBackground->AddChildUI(pLoginButton);

    /* PopupUI */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    if (!pBitmap) return false;
    UIPanel* pPanel = new UIPanel(500, 400, 1920.f / 2, 1080.f / 2, 0.5f, 0.5f);
    pPanel->SetName(L"FailedLogin");
    pPanel->SetBitmap(pBitmap);
    pUIManager->AddUI(pPanel);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_button.png");
    if (!pBitmap) return false;
    UIButton* pOKButton = new UIButton(150, 100, pPanel->GetWidth() / 2, pPanel->GetHeight() / 2 + (pPanel->GetHeight() / 4), 0.5f, 0.5f);
    pOKButton->SetParent(pPanel);
    pOKButton->SetBitmap(pBitmap);
    pOKButton->SetCallback([pPanel]() { pPanel->SetActive(false); UIManager::GetInst()->PopPopupUI(); });
    pOKButton->SetText(L"확인");
    pPanel->AddChildUI(pOKButton);
    pOKButton->SetClickable(true);
    pPanel->SetActive(false);

    return true;
}

void LoginScene::LoginButtonCallback(const std::wstring& _str)
{
    if (_str.length() <= 0 || std::find(_str.begin(), _str.end(), L' ') != _str.end())
    {
        UIPanel* pFailedLoginUI = dynamic_cast<UIPanel*>(UIManager::GetInst()->FindUI(L"FailedLogin"));
        pFailedLoginUI->SetActive(true);
        pFailedLoginUI->SetText(L"잘못된 닉네임 입력입니다.");
        UIManager::GetInst()->AddPopupUI(pFailedLoginUI);
        return;
    }
    EnterPacket packet(_str.c_str());
    NetworkManager::GetInst()->Send(&packet);
}

void LoginScene::EndButtonCallback()
{
}
