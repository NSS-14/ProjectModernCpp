#include "User.h"

User::User(const std::string& name, const std::string& password) : m_name(name), m_password(password)
{
}

const std::string& User::GetName() const
{
	return m_name;
}

const std::string& User::GetPassword() const
{
	return m_password;
}

void User::SetName(const std::string& name)
{
	m_name = name;
}

void User::SetPassword(const std::string& password)
{
	m_password = password;
}
