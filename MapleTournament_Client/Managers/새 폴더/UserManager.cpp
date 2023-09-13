#include "UserManager.h"
#include "../User.h"

UserManager* UserManager::m_pInst = nullptr;

UserManager::UserManager() :
	m_pMyUser(nullptr)
{
}

UserManager::~UserManager()
{
}

User* UserManager::CreateUser(unsigned short _socket, const std::wstring& _nickname)
{
	User* pUser = new User(_socket, _nickname);
	m_vecUser.push_back(pUser);
	return pUser;
}

User* UserManager::GetUser(unsigned short _socket)
{
	for (auto& user : m_vecUser)
	{
		if (user->GetSocket() == _socket)
			return user;
	}
	return nullptr;
}

User* UserManager::GetUserByNickname(const std::wstring& _nickname)
{
	for (auto& user : m_vecUser)
	{
		if (user->GetNickname() == _nickname) 
			return user;
	}
	return nullptr;
}

void UserManager::GetAllUserNickname(std::vector<const std::wstring*>& _vecUserNickname)
{
	for (auto& data : m_vecUser)
	{
		_vecUserNickname.push_back(&data->GetNickname());
	}
}

bool UserManager::RemoveUser(unsigned short _socket)
{
	std::vector<User*>::iterator iter = m_vecUser.begin();
	std::vector<User*>::iterator iterEnd = m_vecUser.end();
	
	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetSocket() == _socket)
		{
			delete *iter;
			m_vecUser.erase(iter);
			return true;
		}
	}
	return false;
}
