#pragma once

#include "Packet.h"

class EnterPacket :
    public Packet
{
public:
    EnterPacket(const wchar_t* _nickname);
    ~EnterPacket();
};

