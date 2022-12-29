#pragma once
#include "DBBehaviour.h"

#include <crow.h>

class LoginHandler {
public:
	LoginHandler(Storage& storage);
	crow::response operator()(const crow::request& request) const;

private:
	Storage& m_db;
	mutable std::map<std::string, User> m_loggedUsers;
};