#include "Room.h"
#include "User.h"

Room::Room(unsigned int _id, const wchar_t* _strTitle)
	: m_id(_id)
{
	m_strTitle = _strTitle;
}

Room::~Room()
{
}

void Room::AddUser(User* _pUser)
{
	m_mapUser.insert(std::make_pair(_pUser->GetNickname(), _pUser));
}
