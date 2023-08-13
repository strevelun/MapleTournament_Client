#include "LoginScene.h"
#include "Layer.h"
#include "../Obj/UI/UIPanel.h"
#include "../Managers/ResourceManager.h"

#include <string>

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

bool LoginScene::Init()
{
    UIPanel* pBackground = new UIPanel();
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"ui_loginscene_background.png", L"Resource\\UI\\");
    if (!pBitmap) return false;
    pBackground->SetBitmap(pBitmap);
    pBackground->SetSize(1920, 1080);

    Layer* pLayer = CreateLayer(L"Background", 0);
    pLayer->AddObj(pBackground);

    return true;
}