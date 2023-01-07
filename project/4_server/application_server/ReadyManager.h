#pragma once

#include <mutex>
#include <cstdint>

class ReadyManager
{
public:
	ReadyManager() = default;

public:
	void SetDesiredNumberOfPlayers(uint8_t value);
	void SetOnlinePlayers(uint8_t value);

public:
	uint8_t GetDesiredNumberOfPlayers() const;
	uint8_t GetOnlinePlayers() const;

private:
	uint8_t m_desiredNumberOfPlayers = 0;
	uint8_t m_onlinePlayers = 0;
	mutable std::mutex m_mutexDesiredNumberOfPlayers;
	mutable std::mutex m_mutexOnlinePlayers;
};

