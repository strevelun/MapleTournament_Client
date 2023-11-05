#pragma once
#include "GameObj.h"

class Graphics;


class Skill :
    public GameObj
{

public:
    Skill();
    virtual ~Skill();

    void Update() override;
    void Render() override;
    virtual void Reset() = 0;
};

