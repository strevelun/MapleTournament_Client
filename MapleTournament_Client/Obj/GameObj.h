#pragma once
#include "Obj.h"

class Animator;

class GameObj :
    public Obj
{
private:
    Animator* m_pAnimator = nullptr;
    
protected:
    D2D1_POINT_2L m_tDestPos; 
    float m_moveSpeed = 3.0f;
    float					m_ratio = 1.0f;

public:
    GameObj();
    virtual ~GameObj();

    void Update() override;
    void Render() override;

    void SetAnimator(Animator* _pAnim);
    void SetRatio(float _ratio) { m_ratio = _ratio; }
};

