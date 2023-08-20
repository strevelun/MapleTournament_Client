#ifndef __SETTING_H__
#define __SETTING_H__

#include <WS2tcpip.h>
#include <winsock2.h>
#include <iostream>

enum class ePacketType
{
	None,
	S_Connect,
	C_Enter,
	S_Enter,
	S_OKLogin,
	S_FailedLogin,
	C_Exit
};

#endif