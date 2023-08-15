#include "LoginScene.h"
#include "Layer.h"
#include "../Obj/UI/UIPanel.h"
#include "../Managers/ResourceManager.h"
#include "../Obj/UI/UIEditText.h"

#include <string>

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

bool LoginScene::Init()
{
    // TODO
    UIPanel* pBackground = new UIPanel();
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_loginscene_background.png");
    if (!pBitmap) return false;
    pBackground->SetBitmap(pBitmap);
    pBackground->SetSize(1920, 1080);

    Layer* pLayer = CreateLayer(L"Background", 0);
    pLayer->AddObj(pBackground);

    UIEditText* pInputNickname = new UIEditText; 
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_loginscene_login_edittext.png");
    if (!pBitmap) return false;
    pInputNickname->SetBitmap(pBitmap);
    pInputNickname->SetSize(500, 50);
    pInputNickname->SetPivot(0.5f, 0.5f);
    pInputNickname->SetPos(pBackground->GetWidth() / 2, pBackground->GetHeight() / 2);

    pLayer = CreateLayer(L"UI", 1);
    pLayer->AddObj(pInputNickname);

    return true;
}

void LoginScene::LoginButtonCallback()
{
}

void LoginScene::EndButtonCallback()
{
}
