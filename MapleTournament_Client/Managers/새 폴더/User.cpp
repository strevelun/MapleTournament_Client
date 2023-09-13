#include "User.h"

User::User(unsigned short _socket, const std::wstring _nickname) :
	m_socket(_socket), m_nickname(_nickname)
{
}

User::~User()
{
}
