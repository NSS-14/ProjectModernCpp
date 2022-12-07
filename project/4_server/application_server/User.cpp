#include "User.h"

User::User(unsigned int id, const std::string& name, const std::string& password) : m_id(id), m_name(name), m_password(password)
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
