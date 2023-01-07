#pragma once

#include <vector>
#include <crow.h>

#include "User.h"
#include "ReadyManager.h"

class LoginHandler {
public:
	LoginHandler(ReadyManager& readyManager);

public:
	crow::response operator()(const crow::request& request) const;

public:
	size_t GetTheNumberOfOnlineUsers() const;
	const std::vector<User>& GetTheOnlineUsers() const;
	User GetTheHost() const;

private:
	mutable std::vector<User> m_onlineUsers; // the first user in this vector is the game host.
	ReadyManager& m_readyManager;
};
