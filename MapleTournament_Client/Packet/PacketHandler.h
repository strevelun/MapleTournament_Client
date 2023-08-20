#pragma once

#include "../Setting.h"

class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();

public:
	void S_Connect(char* _packet);
	void S_FailedLogin();
	void S_OKLogin();
};

