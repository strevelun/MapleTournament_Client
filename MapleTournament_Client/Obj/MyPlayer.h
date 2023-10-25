#pragma once
#include "Player.h"

class MyPlayer :
    public Player
{
public:
    MyPlayer(const wchar_t* _pNickname);
    virtual ~MyPlayer();

    void Update() override;
    void Render() override;

};

