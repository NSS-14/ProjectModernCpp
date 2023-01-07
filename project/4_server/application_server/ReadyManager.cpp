#include "ReadyManager.h"
#include <iostream>

void ReadyManager::SetDesiredNumberOfPlayers(uint8_t value)
{
	std::lock_guard<std::mutex> lock(m_mutexDesiredNumberOfPlayers);
	m_desiredNumberOfPlayers = value;
	std::cout << static_cast<int>(value);
}
void ReadyManager::SetOnlinePlayers(uint8_t value)
{
	std::lock_guard<std::mutex> lock(m_mutexOnlinePlayers);
	m_onlinePlayers = value;
	std::cout << static_cast<int>(value);
}

uint8_t ReadyManager::GetDesiredNumberOfPlayers() const
{
	std::lock_guard<std::mutex> lock(m_mutexDesiredNumberOfPlayers);
	return m_desiredNumberOfPlayers;
}
uint8_t ReadyManager::GetOnlinePlayers() const
{
	std::lock_guard<std::mutex> lock(m_mutexOnlinePlayers);
	return m_onlinePlayers;
}
