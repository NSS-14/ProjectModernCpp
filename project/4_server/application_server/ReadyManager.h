#pragma once

#include <mutex>
#include <cstdint>
#include <vector>

#include "User.h"

class ReadyManager
{
public:
	ReadyManager() = default;

public:
	void SetDesiredNumberOfPlayers(uint8_t value);

public:
	void AddUser(const User& user);
	void AddUser(User&& user);
	void AddUser(unsigned int id, const std::string& name, const std::string& password);

public:
	const std::vector<User>& GetUsers() const;
	uint8_t GetDesiredNumberOfPlayers() const;
	uint8_t GetOnlinePlayers() const;

private:
	std::vector<User> m_users;
	uint8_t m_desiredNumberOfPlayers = 0;
	mutable std::mutex m_mutexUsers;
	mutable std::mutex m_mutexDesiredNumberOfPlayers;
};

