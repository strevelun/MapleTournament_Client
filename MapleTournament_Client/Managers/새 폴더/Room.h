#pragma once

#include <map>
#include <string>

class User;

enum class eRoomState
{
	None,
	Ready,
	InGame
};

class Room
{
	unsigned int m_id;
	eRoomState m_eState = eRoomState::Ready;
	std::wstring m_strTitle;

	std::map<std::wstring, User*> m_mapUser;

public:
	Room(unsigned int _id, const wchar_t* _strTitle);
	~Room();

	void AddUser(User* _pUser);
	unsigned int GetId() const { return m_id; }
};

