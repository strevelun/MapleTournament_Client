#pragma once

#include <string>

class User
{
private:
	unsigned short 	m_socket;
	std::wstring m_nickname;

public:
	User(unsigned short _socket, std::wstring _nickname);
	~User();

	unsigned short GetSocket() const { return m_socket; }
	const std::wstring& GetNickname() const { return m_nickname; }
};

