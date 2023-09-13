#include "RoomManager.h"
#include "../Room.h"

RoomManager* RoomManager::m_inst = nullptr;

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

Room* RoomManager::CreateRoom(unsigned int _roomId, const wchar_t* _strTitle)
{
	Room* pRoom = new Room(_roomId, _strTitle);
	m_mapRoom.insert(std::make_pair(_roomId++, pRoom));

	return pRoom;
}
