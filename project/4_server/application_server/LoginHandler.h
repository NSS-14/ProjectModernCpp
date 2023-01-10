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

private:
	ReadyManager& m_readyManager;
};
