#pragma once

#include <string>
#include <vector>

class User;

class UserManager
{
private:
	static UserManager* m_pInst;

	//std::map<unsigned short, User*> m_mapUser;
	std::vector<User*> m_vecUser;

	User* m_pMyUser;

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

	User* CreateUser(unsigned short _socket, const std::wstring& _nickname);
	User* GetUser(unsigned short _socket);
	User* GetUserByNickname(const std::wstring& _nickname);
	User* GetMyUser() const { return m_pMyUser; }
	void GetAllUserNickname(std::vector<const std::wstring*>& _vecUserNickname);
	
	void SetMyUser(User* _pThisUser) { m_pMyUser = _pThisUser; }

	bool RemoveUser(unsigned short _socket);
};

