#include "UserManager.h"

UserManager* UserManager::m_pInst = nullptr;

UserManager::UserManager() :
	m_pMyUser(nullptr)
{
}

UserManager::~UserManager()
{
}

bool UserManager::AddUser(uint16_t _id, User* _pUser)
{
	if (m_mapUser.find(_id) != m_mapUser.end())		return false;

	m_mapUser.insert({ _id, _pUser });
	return true;
}

User* UserManager::GetUser(uint16_t _id)
{
	std::map<uint16_t, User*>::const_iterator pUser = m_mapUser.find(_id);
	if (pUser == m_mapUser.end()) return nullptr;

	return pUser->second;
}
