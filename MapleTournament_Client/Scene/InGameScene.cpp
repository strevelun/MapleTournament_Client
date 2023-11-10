#include "../NetworkCore/NetworkManager.h"
#include "InGameScene.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ObjectManager.h"
#include "../Managers/UIManager.h"
#include "../Constants.h"
#include "../Obj/UI/UIPanel.h"
#include "../Obj/UI/UIButton.h"
#include "../Obj/UI/UIText.h"
#include "../Obj/Player.h"
#include "../Obj/Skill.h"
#include "../Animation/Animator.h"
#include "../Animation/AnimationClip.h"
#include "LobbyScene.h"
#include "../Managers/SceneManager.h"
#include "Layer.h"
#include "../Timer.h"
#include "../Debug.h"

// x : 200~1080(880)
// y : 200~600

typedef unsigned short ushort;

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
    ObjectManager::GetInst()->ClearAll();
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
    UIPanel* pPanel = new UIPanel(nullptr, 300, 100);
    pPanel->SetName(L"PlayerStat0");
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingamescene_stat.png");
    if (pBitmap) pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddUI(pPanel);
    UIPanel* pPicture = new UIPanel(pPanel, 90, 80, 10, 10);
    pPicture->SetName(L"Picture");
    pPanel->AddChildUI(pPicture);
    UIText* pPlayerName = new UIText(pPanel, L"", 30.f, 130, 10);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);
    UIText* pHP = new UIText(pPanel, L"HP : 0", 15.f, 130, 40);
    pHP->SetName(L"HP");
    pPanel->AddChildUI(pHP);
    UIText* pMP = new UIText(pPanel, L"MP : 0", 15.f, 130, 70);
    pMP->SetName(L"MP");
    pPanel->AddChildUI(pMP);
    pPanel->SetActive(false);

    pPanel = new UIPanel(nullptr, 300, 100, ScreenWidth, 0, 1.0f);
    pPanel->SetName(L"PlayerStat1");
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingamescene_stat.png");
    if (pBitmap) pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddUI(pPanel);
    pPicture = new UIPanel(pPanel, 90, 80, 10, 10);
    pPicture->SetName(L"Picture");
    pPanel->AddChildUI(pPicture);
    pPlayerName = new UIText(pPanel, L"", 30.f, 130, 10);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);
    pHP = new UIText(pPanel, L"HP : 0", 15.f, 130, 40);
    pHP->SetName(L"HP");
    pPanel->AddChildUI(pHP);
    pMP = new UIText(pPanel, L"MP : 0", 15.f, 130, 70);
    pMP->SetName(L"MP");
    pPanel->AddChildUI(pMP);
    pPanel->SetActive(false);

    pPanel = new UIPanel(nullptr, 300, 100, 0, ScreenHeight, 0.0f, 1.0f);
    pPanel->SetName(L"PlayerStat2");
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingamescene_stat.png");
    if (pBitmap) pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddUI(pPanel);
    pPicture = new UIPanel(pPanel, 90, 80, 10, 10);
    pPicture->SetName(L"Picture");
    pPanel->AddChildUI(pPicture);
    pPlayerName = new UIText(pPanel, L"", 30.f, 130, 10);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);
    pHP = new UIText(pPanel, L"HP : 0", 15.f, 130, 40);
    pHP->SetName(L"HP");
    pPanel->AddChildUI(pHP);
    pMP = new UIText(pPanel, L"MP : 0", 15.f, 130, 70);
    pMP->SetName(L"MP");
    pPanel->AddChildUI(pMP);
    pPanel->SetActive(false);

    pPanel = new UIPanel(nullptr, 300, 100, ScreenWidth, ScreenHeight, 1.0f, 1.0f);
    pPanel->SetName(L"PlayerStat3");
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingamescene_stat.png");
    if (pBitmap) pPanel->SetBitmap(pBitmap);
    UIManager::GetInst()->AddUI(pPanel);
    pPicture = new UIPanel(pPanel, 90, 80, 10, 10);
    pPicture->SetName(L"Picture");
    pPanel->AddChildUI(pPicture);
    pPlayerName = new UIText(pPanel, L"", 30.f, 130, 10);
    pPlayerName->SetName(L"Nickname");
    pPanel->AddChildUI(pPlayerName);
    pHP = new UIText(pPanel, L"HP : 0", 15.f, 130, 40);
    pHP->SetName(L"HP");
    pPanel->AddChildUI(pHP);
    pMP = new UIText(pPanel, L"MP : 0", 15.f, 130, 70);
    pMP->SetName(L"MP");
    pPanel->AddChildUI(pMP);
    pPanel->SetActive(false);

    /* 대기하세요 UI */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    pPanel = new UIPanel(nullptr, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    if (pBitmap) 
        pPanel->SetBitmap(pBitmap);
    pPanel->SetName(L"Standby");
    pPanel->SetActive(false);
    UIText* pText = new UIText(pPanel, L"대기하세요", 20.f, pPanel->GetWidth() / 2, 100, 0.5f, 0.5f);
    pPanel->AddChildUI(pText);
    UIManager::GetInst()->AddUI(pPanel);

    /* 게임 종료 UI */
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_board.png");
    pPanel = new UIPanel(nullptr, 500, 400, ScreenWidth / 2, ScreenHeight / 2, 0.5f, 0.5f);
    if (pBitmap) 
        pPanel->SetBitmap(pBitmap);
    pPanel->SetName(L"GameOver");
    pPanel->SetActive(false);
    pText = new UIText(pPanel, L"게임오버!", 20.f, pPanel->GetWidth() / 2, 100, 0.5f, 0.5f);
    pPanel->AddChildUI(pText);
    UIManager::GetInst()->AddUI(pPanel);

    /* 대시보드 */
    pPanel = new UIPanel(nullptr, 100, 50, ScreenWidth / 2, 0, 0.5f);
    pPanel->SetName(L"Dashboard");
    UIManager::GetInst()->AddUI(pPanel);
    UIText *pDashboardText = new UIText(pPanel, L"1 / " + std::to_wstring(GAME_MAX_TURN), 30.f);
    pPanel->AddChildUI(pDashboardText);
    pDashboardText->SetName(L"DashboardText");

    /* 타이머 UI */
    for (int i = 0; i <= 9; i++)
    {
        pPanel = new UIPanel(nullptr, 100, 100, ScreenWidth / 2, 100, 0.5f);
        pPanel->SetActive(false);
        pPanel->SetName(std::to_wstring(i));
        pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_" + std::to_wstring(i) + L".png");
        if (pBitmap) pPanel->SetBitmap(pBitmap);
        UIManager::GetInst()->AddUI(pPanel);
        m_arrTimer[i] = pPanel;
    }

    UIPanel* pPanelWait = new UIPanel(nullptr, 100, 50, ScreenWidth / 2, 100, 0.5f);
    pPanelWait->SetActive(true);
    pPanelWait->SetName(L"Wait");
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_wait.png");
    if (pBitmap) pPanelWait->SetBitmap(pBitmap);
    UIManager::GetInst()->AddUI(pPanelWait);

    /* 스킬 */
    Skill* pSkill = new Skill;
    AnimationClip* pClip = ResourceManager::GetInst()->GetAnimClip(L"attack0", L"attack0");
    if (pClip)
    {
        Animator* pAnimator = new Animator(pClip);
        pClip->SetLoop(false);
        pClip->SetPlayTime(5.f);
        pClip->SetAnyState(false);
        pSkill->SetAnimator(pAnimator);
    }
    pSkill->SetActive(false);
    ObjectManager::GetInst()->AddSkill(pSkill, eSkillName::Attack0);

    pLayer = new Layer(L"Skill", 2);
    m_vecObjLayer.push_back(pLayer);
    pLayer->AddObj(pSkill);

    pSkill = new Skill;
    pClip = ResourceManager::GetInst()->GetAnimClip(L"attack1", L"attack1");
    if (pClip)
    {
        Animator* pAnimator = new Animator(pClip);
        pClip->SetLoop(false);
        pClip->SetPlayTime(5.f);
        pClip->SetAnyState(false);
        pSkill->SetAnimator(pAnimator);
    }
    pSkill->SetActive(false);
    ObjectManager::GetInst()->AddSkill(pSkill, eSkillName::Attack1);
    pLayer->AddObj(pSkill);

    pSkill = new Skill;
    pClip = ResourceManager::GetInst()->GetAnimClip(L"attack2", L"attack2");
    if (pClip)
    {
        Animator* pAnimator = new Animator(pClip);
        pClip->SetLoop(false);
        pClip->SetPlayTime(5.f);
        pClip->SetAnyState(false);
        pSkill->SetAnimator(pAnimator);
    }
    pSkill->SetActive(false);
    ObjectManager::GetInst()->AddSkill(pSkill, eSkillName::Attack2);
    pLayer->AddObj(pSkill);

    pSkill = new Skill;
    pClip = ResourceManager::GetInst()->GetAnimClip(L"attack3", L"attack3");
    if (pClip)
    {
        Animator* pAnimator = new Animator(pClip);
        pClip->SetLoop(false);
        pClip->SetLoopCount(3);
        pClip->SetPlayTime(2.f);
        pClip->SetAnyState(false);
        pSkill->SetAnimator(pAnimator);
    }
    pSkill->SetActive(false);
    ObjectManager::GetInst()->AddSkill(pSkill, eSkillName::Attack3);
    pLayer->AddObj(pSkill);

    pSkill = new Skill;
    pClip = ResourceManager::GetInst()->GetAnimClip(L"heal0", L"heal0");
    if (pClip)
    {
        Animator* pAnimator = new Animator(pClip);
        pClip->SetLoop(false);
        pClip->SetPlayTime(2.f);
        pClip->SetAnyState(false);
        pSkill->SetAnimator(pAnimator);
    }
    pSkill->SetActive(false);
    ObjectManager::GetInst()->AddSkill(pSkill, eSkillName::Heal0);
    pLayer->AddObj(pSkill);
    

    /* 스킬 버튼 */
    pPanel = new UIPanel(nullptr, 500, 100, ScreenWidth / 2, ScreenHeight + 80, 0.5f, 1.0f);
    pPanel->SetName(L"SkillButtonPanel");
    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_leftmove.png");
    UIButton* pButton = new UIButton(pPanel, 50, 50, 0, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetCallback([this, pPanel] 
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Move, eMoveName::LeftMove);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_leftdoublemove.png");
    pButton = new UIButton(pPanel, 50, 50, 50, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Move, eMoveName::LeftDoubleMove);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_rightmove.png");
    pButton = new UIButton(pPanel, 50, 50, 100, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Move, eMoveName::RightMove);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_rightdoublemove.png");
    pButton = new UIButton(pPanel, 50, 50, 150, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Move, eMoveName::RightDoubleMove);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_upmove.png");
    pButton = new UIButton(pPanel, 50, 50, 200, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Move, eMoveName::UpMove);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_downmove.png");
    pButton = new UIButton(pPanel, 50, 50, 250, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Move, eMoveName::DownMove);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_button_attack0.png");
    pButton = new UIButton(pPanel, 50, 50, 300, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetName(std::to_wstring((int)eSkillName::Attack0));
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Skill, eSkillName::Attack0);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_button_attack1.png");
    pButton = new UIButton(pPanel, 50, 50, 350, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetName(std::to_wstring((int)eSkillName::Attack1));
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Skill, eSkillName::Attack1);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_button_attack2.png");
    pButton = new UIButton(pPanel, 50, 50, 400, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetName(std::to_wstring((int)eSkillName::Attack2));
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Skill, eSkillName::Attack2);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_button_attack3.png");
    pButton = new UIButton(pPanel, 50, 50, 450, 0);
    pButton->SetBitmap(pBitmap);
    pButton->SetName(std::to_wstring((int)eSkillName::Attack3));
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Skill, eSkillName::Attack3);
        });
    pPanel->AddChildUI(pButton);

    pBitmap = ResourceManager::GetInst()->GetBitmap(L"Resource\\UI\\ui_ingame_button_heal0.png");
    pButton = new UIButton(pPanel, 50, 50, 0, 50);
    pButton->SetBitmap(pBitmap);
    pButton->SetCallback([this, pPanel]
        {
            OnItemButtonClick(pPanel);
            SendActionPacket(eActionType::Skill, eSkillName::Heal0);
        });
    pPanel->AddChildUI(pButton);

    UIManager::GetInst()->AddUI(pPanel);

    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_InGameReady;          count += sizeof(ushort);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);

    return true;
}

void InGameScene::Update()
{
    Scene::Update();

    if (m_eState == eInGameState::None) return;

    if (m_eState == eInGameState::Play)
    {
        if (!m_isMyTurn) return;

        m_arrTimer[(u_int)m_timer]->SetActive(false);
        m_timer -= Timer::GetInst()->GetDeltaTime();
        if (m_timer <= 0.f)
        {
            OnTimeout();
            return;
        }
        m_arrTimer[(u_int)m_timer]->SetActive(true);
    }
    else if (m_eState == eInGameState::Prepare)
    {
        m_timer -= Timer::GetInst()->GetDeltaTime();
        if (m_timer <= 0.f)
        {
            UI* pUI = UIManager::GetInst()->FindUI(L"Standby");
            if (!pUI) return;
            UIPanel* pPanel = static_cast<UIPanel*>(pUI);
            pPanel->SetActive(false);
            UIManager::GetInst()->PopPopupUI();
            ChangeState(eInGameState::None);

            char buffer[255];
            ushort count = sizeof(ushort);
            *(ushort*)(buffer + count) = (ushort)ePacketType::C_Standby;          count += sizeof(ushort);
            *(ushort*)buffer = count;
            NetworkManager::GetInst()->Send(buffer);
        }
    }
    else if (m_eState == eInGameState::GameOver)
    {
        m_timer -= Timer::GetInst()->GetDeltaTime();
        if (m_timer <= 0.f)
        {
            UI* pUI = UIManager::GetInst()->FindUI(L"GameOver");
            if (!pUI) return;
            UIPanel* pPanel = static_cast<UIPanel*>(pUI);
            pPanel->SetActive(false);
            UIManager::GetInst()->PopPopupUI();
            ChangeState(eInGameState::None);

            LobbyScene* pScene = new LobbyScene;
            SceneManager::GetInst()->ChangeScene(pScene);

            char buffer[255];
            ushort count = sizeof(ushort);
            *(ushort*)(buffer + count) = (ushort)ePacketType::C_GameOver;				count += sizeof(ushort);
            *(ushort*)buffer = count;
            NetworkManager::GetInst()->Send(buffer);
        }
    }
    else if (m_eState == eInGameState::UseSkill)
    {
       if (m_eSkillState == eSkillState::CheckHit)
        {
            if (m_isMyTurn)
            {
                char buffer[255];
                ushort count = sizeof(ushort);
                *(ushort*)(buffer + count) = (ushort)ePacketType::C_CheckHit;               count += sizeof(ushort);
                *(ushort*)buffer = count;
                NetworkManager::GetInst()->Send(buffer);
            }
            m_eSkillState = eSkillState::None;
        }
       else if (m_eSkillState == eSkillState::CheckHeal)
       {
           char buffer[255];
           ushort count = sizeof(ushort);
           *(ushort*)(buffer + count) = (ushort)ePacketType::C_CheckHeal;               count += sizeof(ushort);
           *(ushort*)buffer = count;
           NetworkManager::GetInst()->Send(buffer);
       }
       else if (m_eSkillState == eSkillState::End)
       {
            if (m_isMyTurn)
            {
                NextTurn();
                SetMyTurn(false);
            }
            m_eSkillState = eSkillState::None;
       }
    }
}

void InGameScene::SetMyTurn(bool _isMyTurn)
{
    m_isMyTurn = _isMyTurn;
}

void InGameScene::ChangeState(eInGameState _state)
{
    m_eState = _state;
    switch (_state)
    {
    case eInGameState::Prepare:
        m_timer = PrepareTimer;
        break;
    case eInGameState::Play:
        m_timer = StartTimer;
        break;
    case eInGameState::GameOver:
        m_timer = GameOverTimer;
        break;
    }
}
/*
void InGameScene::SetSkillTimer(float _timer)
{
    if (m_eState != eInGameState::UseSkill) return;

    m_timer = _timer;
}
*/
void InGameScene::SendActionPacket(eActionType _type, eMoveName _name)
{
    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_Skill;				count += sizeof(ushort);
    *(char*)(buffer + count) = (char)_type;                     count += sizeof(char);
    *(char*)(buffer + count) = (char)_name;                                 count += sizeof(char);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void InGameScene::SendActionPacket(eActionType _type, eSkillName _name)
{
    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_Skill;				count += sizeof(ushort);
    *(char*)(buffer + count) = (char)_type;                     count += sizeof(char);
    *(char*)(buffer + count) = (char)_name;                                 count += sizeof(char);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void InGameScene::NextTurn()
{
    char buffer[255];
    ushort count = sizeof(ushort);
    *(ushort*)(buffer + count) = (ushort)ePacketType::C_NextTurn;				count += sizeof(ushort);
    *(ushort*)buffer = count;
    NetworkManager::GetInst()->Send(buffer);
}

void InGameScene::OnItemButtonClick(UIPanel* _pPanel)
{
    UI* pUI = UIManager::GetInst()->FindUI(L"Wait");
    if (pUI)
    {
        UIPanel* pPanel = static_cast<UIPanel*>(pUI);
        pPanel->SetActive(true);
    }
    _pPanel->SetActive(true);
    _pPanel->SetClickable(false);
    _pPanel->SetPos(ScreenWidth / 2, ScreenHeight + 80);
    m_arrTimer[(u_int)m_timer]->SetActive(false);

    ChangeState(eInGameState::None);
    // 스킬 애니메이션 출력 후 NextTurn
}

void InGameScene::OnTimeout()
{
    UI* pUI = UIManager::GetInst()->FindUI(L"SkillButtonPanel");
    if (!pUI) return;
    pUI->SetPos(ScreenWidth / 2, ScreenHeight + 80);
    pUI->SetClickable(false);

    pUI = UIManager::GetInst()->FindUI(L"Wait");
    if (pUI)
    {
        UIPanel* pPanel = static_cast<UIPanel*>(pUI);
        pPanel->SetActive(true);
    }

    SetMyTurn(false);
    m_timer = StartTimer;
    NextTurn();
}
