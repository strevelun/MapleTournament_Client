#include "ExitPacket.h"
#include "../Setting.h"

ExitPacket::ExitPacket()
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_packetBuffer + count) = (ushort)ePacketType::C_Exit;										count += sizeof(ushort);
	*(ushort*)m_packetBuffer = count;
}

ExitPacket::~ExitPacket()
{
}
