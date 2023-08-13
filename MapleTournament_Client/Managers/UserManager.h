#pragma once

#include <map>

class UserManager
{
private:
	static UserManager* m_pInst;

	std::map<uint16_t, class User*> m_mapUser;

	class User* m_pMyUser;

	UserManager();
	~UserManager();

public:
	static UserManager* GetInst()
	{
		if (!m_pInst) m_pInst = new UserManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
		m_pInst = nullptr;
	}

	bool AddUser(uint16_t _id, User* _pUser);
	User* GetUser(uint16_t _id);
	User* GetThisUser() const { return m_pMyUser; }
	void SetMyUser(User* _pThisUser) { m_pMyUser = _pThisUser; }
};

