#include "InGameScene.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ObjectManager.h"
#include "../Managers/UIManager.h"
#include "../Constants.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIButton.h"
#include "../Obj/MyPlayer.h"
#include "../Animation/Animator.h"
#include "../Animation/AnimationClip.h"
#include "Layer.h"

// x : 200~1080(880)
// y : 200~600

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
}

bool InGameScene::Init()
{
    /* Background */;

    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"ui_ingame_scene");
    if (!pBitmap) return false;
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    Layer* pLayer = new Layer(L"Background", 0);
    pLayer->AddObj(pBackground);
    m_vecObjLayer.push_back(pLayer);

    AnimationClip* pTestClip = ResourceManager::GetInst()->GetAnimClip(L"Attack1");
    pTestClip->SetLoop(false);
    pTestClip->SetPlayTime(3.0f);
    pTestClip->SetAnyState(true);
    Animator* pAnimator = new Animator(pTestClip);

    MyPlayer* myPlayer = new MyPlayer(L"Test");
    myPlayer->SetPos(100, 100);
    myPlayer->SetSize(200, 200);
    myPlayer->SetAnimator(pAnimator);
    ObjectManager::GetInst()->SetMyPlayer(myPlayer);
    
    pLayer = new Layer(L"Player", 1);
    pLayer->AddObj(myPlayer);
    m_vecObjLayer.push_back(pLayer);

    return true;
}
