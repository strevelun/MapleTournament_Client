#include "../NetworkCore/NetworkManager.h"
#include "InGameScene.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ObjectManager.h"
#include "../Managers/UIManager.h"
#include "../Constants.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIButton.h"
#include "../Obj/UI/UIText.h"
#include "../Obj/MyPlayer.h"
#include "../Obj/SkillMove.h"
#include "../Obj/SkillAttackCloud.h"
#include "../Animation/Animator.h"
#include "../Animation/AnimationClip.h"
#include "Layer.h"

// x : 200~1080(880)
// y : 200~600

typedef unsigned short ushort;

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
}

bool InGameScene::Init()
{
    /* Background */

    Bitmap* pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_scene.png");
    UIPanel* pBackground = new UIPanel(nullptr, ScreenWidth, ScreenHeight);
    pBackground->SetBitmap(pBitmap);
    Layer* pLayer = new Layer(L"Background", 0);
    pLayer->AddObj(pBackground);
    m_vecObjLayer.push_back(pLayer);
    
    pLayer = new Layer(L"Player", 1);
    m_vecObjLayer.push_back(pLayer);



    /* 플레이어 스텟 */
    UIPanel* pPanel = new UIPanel(nullptr, 200, 50);
    pPanel->SetName(L"PlayerStat0");
    UIManager::GetInst()->AddUI(pPanel);
    UIText* pPlayerName = new UIText(pPanel, L"", 20.f, 200, 50);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);

    pPanel = new UIPanel(nullptr, 200, 50, ScreenWidth, 0, 1.0f);
    pPanel->SetName(L"PlayerStat1");
    UIManager::GetInst()->AddUI(pPanel);
    pPlayerName = new UIText(pPanel, L"", 20.f, 200, 50);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);

    pPanel = new UIPanel(nullptr, 200, 50, 0, ScreenHeight, 0.0f, 1.0f);
    pPanel->SetName(L"PlayerStat2");
    UIManager::GetInst()->AddUI(pPanel);
    pPlayerName = new UIText(pPanel, L"", 20.f, 200, 50);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);

    pPanel = new UIPanel(nullptr, 200, 50, ScreenWidth, ScreenHeight, 1.0f, 1.0f);
    pPanel->SetName(L"PlayerStat3");
    UIManager::GetInst()->AddUI(pPanel);
    pPlayerName = new UIText(pPanel, L"", 20.f, 200, 50);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);

    /* 스킬 */
    AnimationClip* pClip = ResourceManager::GetInst()->GetAnimClip(L"attackCloud");
    pClip->SetLoop(true);
    pClip->SetPlayTime(1.5f);
    pClip->SetAnyState(true);
    Animator* pAnimator = new Animator(pClip);

    SkillAttackCloud* pSkill = new SkillAttackCloud(2.0f);
    pSkill->SetAnimator(pAnimator);
    ObjectManager::GetInst()->AddSkill(pSkill, eSkillType::AttackCloud);

    pLayer = new Layer(L"Skill", 2);
    //pLayer->AddObj(pSkill);
    m_vecObjLayer.push_back(pLayer);

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_InGameReady;          count += sizeof(ushort);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);

    return true;
}
