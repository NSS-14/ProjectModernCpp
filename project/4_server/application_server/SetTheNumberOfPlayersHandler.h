#pragma once

#include <crow.h>

#include "ReadyManager.h"

class SetTheNumberOfPlayersHandler
{
public:
	SetTheNumberOfPlayersHandler(ReadyManager& readyManager);

public:
	crow::response operator ()(const crow::request& request) const;

public:
	uint8_t GetNumberOfPlayer() const;

private:
	ReadyManager& m_readyManager;
};
