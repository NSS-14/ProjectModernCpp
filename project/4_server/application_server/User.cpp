#include "User.h"

User::User(unsigned int id, const std::string& name, const std::string& password) 
	: m_id(id)
	, m_name(name)
	, m_password(password)
{
}

User::User(const User& user)
{
	*this = user;
}

User& User::operator=(const User& user)
{
	m_id = user.m_id;
	m_name = user.m_name;
	m_password = user.m_password;
	return *this;
}

bool User::operator==(const User& user)
{
	return m_id == user.m_id
		&& m_name == user.m_name
		&& m_password == user.m_password;
}

const std::string& User::GetName() const
{
	return m_name;
}

const std::string& User::GetPassword() const
{
	return m_password;
}

unsigned int User::GetId() const
{
	return m_id;
}

void User::SetId(unsigned int id)
{
	m_id = id;
}

void User::SetName(const std::string& name)
{
	m_name = name;
}

void User::SetPassword(const std::string& password)
{
	m_password = password;
}
