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

    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_scene.png");
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    Layer* pLayer = new Layer(L"Background", 0);
    pLayer->AddObj(pBackground);
    m_vecObjLayer.push_back(pLayer);

    AnimationClip* pTestClip = ResourceManager::GetInst()->GetAnimClip(L"Player1");
    pTestClip->SetLoop(false);
    pTestClip->SetPlayTime(3.0f);
    pTestClip->SetAnyState(true);
    //pTestClip->SetFlip(true);
    Animator* pAnimator = new Animator(pTestClip);

    MyPlayer* myPlayer = new MyPlayer(L"Test");
    myPlayer->SetPos(240, 280);
    myPlayer->SetRatio(1.5f);
    myPlayer->SetAnimator(pAnimator);
    ObjectManager::GetInst()->SetMyPlayer(myPlayer);
    
    pLayer = new Layer(L"Player", 1);
    pLayer->AddObj(myPlayer);
    m_vecObjLayer.push_back(pLayer);

    /* 스킬 버튼 */
    UIPanel* pPanel = new UIPanel(nullptr, 500, 100, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    pPanel->SetName(L"SkillButtonPanel");
    UIManager::GetInst()->AddUI(pPanel);
    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_leftmove.png");
    UIButton* pButton = new UIButton(pPanel, 50, 50);
    pButton->SetBitmap(pBitmap);
    pButton->SetClickable(true);
    //pButton->SetCallback()
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_leftdoublemove.png");
    UIButton* pButton = new UIButton(pPanel, 50, 50);
    pButton->SetBitmap(pBitmap);
    pButton->SetClickable(true);
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_button_attackCloud.png");
    pButton->SetBitmap(pBitmap);
    pButton->SetClickable(true);
    pPanel->AddChildUI(pButton);

    AnimationClip* pTestClip = ResourceManager::GetInst()->GetAnimClip(L"attackCloud");
    pTestClip->SetLoop(true);
    pTestClip->SetPlayTime(3.0f);
    pTestClip->SetAnyState(true);
    Animator* pAnimator = new Animator(pTestClip);


    return true;
}
