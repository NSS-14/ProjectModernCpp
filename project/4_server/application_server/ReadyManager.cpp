#include "ReadyManager.h"
#include <iostream>

void ReadyManager::SetDesiredNumberOfPlayers(uint8_t value)
{
	std::lock_guard<std::mutex> lock(m_mutexDesiredNumberOfPlayers);
	m_desiredNumberOfPlayers = value;
}

void ReadyManager::AddUser(const User& user)
{
	std::lock_guard<std::mutex> lock(m_mutexUsers);
	m_users.push_back(user);
}
void ReadyManager::AddUser(User&& user)
{
	std::lock_guard<std::mutex> lock(m_mutexUsers);
	m_users.emplace_back(std::move(user));
}
void ReadyManager::AddUser(unsigned int id, const std::string& name, const std::string& password)
{
	std::lock_guard<std::mutex> lock(m_mutexUsers);
	m_users.emplace_back(id, name, password);
}

const std::vector<User>& ReadyManager::GetUsers() const
{
	std::lock_guard<std::mutex> lock(m_mutexUsers);
	return m_users;
}
uint8_t ReadyManager::GetDesiredNumberOfPlayers() const
{
	std::lock_guard<std::mutex> lock(m_mutexDesiredNumberOfPlayers);
	return m_desiredNumberOfPlayers;
}
uint8_t ReadyManager::GetOnlinePlayers() const
{
	std::lock_guard<std::mutex> lock(m_mutexUsers);
	return m_users.size();
}
