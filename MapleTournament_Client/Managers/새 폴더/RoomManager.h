#pragma once

#include <map>

class Room;

class RoomManager
{
private:
	static RoomManager* m_inst;

	std::map<unsigned int, Room*> m_mapRoom;

	Room* m_pJoinedRoom;

	RoomManager();
	~RoomManager();

public:
	static RoomManager* GetInst()
	{
		if (!m_inst)
			m_inst = new RoomManager;
		return m_inst;
	}

	static void DestroyInst()
	{
		if (m_inst)
			delete m_inst;
		m_inst = nullptr;
	}

	Room* CreateRoom(unsigned int _roomId, const wchar_t* _strTitle);
};

