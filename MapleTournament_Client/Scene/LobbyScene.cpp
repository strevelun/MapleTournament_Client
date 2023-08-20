#include "LobbyScene.h"
#include "../Managers/UIManager.h"
#include "../Managers/ResourceManager.h"
#include "../Bitmap.h"
#include "../Obj/UI/UIPanel.h"

LobbyScene::LobbyScene()
{
}

LobbyScene::~LobbyScene()
{
}

bool LobbyScene::Init()
{
    UIManager* pUIManager = UIManager::GetInst();
    /* Background */;
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_loginscene_background.png");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(1920, 1080);
    pBackground->SetBitmap(pBitmap);
    pUIManager->AddUI(pBackground);

    return true;
}
