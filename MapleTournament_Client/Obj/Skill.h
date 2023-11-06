#pragma once
#include "GameObj.h"

class Graphics;


class Skill :
    public GameObj
{
private:
    bool            m_isEnd = false;

public:
    Skill();
    virtual ~Skill();

    bool IsEnd() const { return m_isEnd; }

    void Update() override;
    void Render() override;
    void Reset();
    void SetDir(eDir _eDir) override;
};

