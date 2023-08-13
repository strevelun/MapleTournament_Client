#include "EnterPacket.h"
#include "../Managers/UserManager.h"
#include "../User.h"

#pragma warning(disable : 6305)

EnterPacket::EnterPacket(const wchar_t* _nickname) // 문자열 끝에 널문자 있어야 함
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_packetBuffer + count) = (ushort)ePacketType::C_Enter;										count += sizeof(ushort);
	*(ushort*)(m_packetBuffer + count) = (ushort)UserManager::GetInst()->GetThisUser()->GetId();			count += sizeof(ushort);
	memcpy(m_packetBuffer + count, _nickname, wcslen(_nickname) * 2);										count += (ushort)wcslen(_nickname) * 2;
	*(char*)(m_packetBuffer + count) = 0;																	count += 1;
	
	*(ushort*)m_packetBuffer = count;
}

EnterPacket::~EnterPacket()
{
}
