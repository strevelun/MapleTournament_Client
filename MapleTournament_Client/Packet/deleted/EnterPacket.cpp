#include "EnterPacket.h"
#include "../Managers/UserManager.h"
#include "../User.h"
#include "../Setting.h"

#pragma warning(disable : 6305)

EnterPacket::EnterPacket(const wchar_t* _nickname) // 문자열 끝에 널문자 있어야 함
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_packetBuffer + count) = (ushort)ePacketType::C_Enter;										count += sizeof(ushort);
	memcpy(m_packetBuffer + count, _nickname, wcslen(_nickname) * 2);										count += (ushort)wcslen(_nickname) * 2;
	*(wchar_t*)(m_packetBuffer + count) = L'\0';																	count += 2;
	*(ushort*)m_packetBuffer = count;
}

EnterPacket::~EnterPacket()
{
}
