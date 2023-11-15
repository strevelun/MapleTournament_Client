#pragma once
#include "../Obj.h"

class Animator;

enum class eDir
{
    Left = -1,
    None,
    Right
};

class GameObj :
    public Obj
{
protected:
    eDir m_eDir = eDir::None;

protected:
    Animator* m_pAnimator = nullptr;
    D2D1_POINT_2F					m_tDestPos;
    float                       m_moveSpeed = 1;
    float                       m_time = 0.f;
    
protected:
    float					m_ratio = 1.0f;

public:
    GameObj();
    virtual ~GameObj();

    virtual void SetDir(eDir _eDir);

    void Update() override;
    void Render() override;

    void SetAnimator(Animator* _pAnim);
    void SetRatio(float _ratio) { m_ratio = _ratio; }
};

