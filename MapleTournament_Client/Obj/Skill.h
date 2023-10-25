#pragma once
#include "GameObj.h"

class Graphics;

enum class eDir
{
    Left = -1,
    None,
    Right
};

class Skill :
    public GameObj
{
private:
    eDir m_dir = eDir::None;

public:
    Skill();
    virtual ~Skill();

    void SetDir(eDir _eDir) { m_dir = _eDir; }

    void Update() override;
    void Render() override;
    virtual void Reset() = 0;
};

