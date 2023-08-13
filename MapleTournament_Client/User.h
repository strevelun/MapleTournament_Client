#pragma once
class User
{
private:
	unsigned short m_id;
	wchar_t* m_nickname;

public:
	User(unsigned short _id);
	~User();

	unsigned short GetId() const { return m_id; }
};

