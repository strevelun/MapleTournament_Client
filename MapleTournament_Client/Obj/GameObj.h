#pragma once
#include "Obj.h"

class Animator;

class GameObj :
    public Obj
{
protected:
    Animator* m_pAnimator = nullptr;
    
protected:
    //float m_moveSpeed = 3.0f;
    float					m_ratio = 1.0f;

public:
    GameObj();
    virtual ~GameObj();

    void Update() override;
    void Render() override;

    void SetAnimator(Animator* _pAnim);
    void SetRatio(float _ratio) { m_ratio = _ratio; }
};

