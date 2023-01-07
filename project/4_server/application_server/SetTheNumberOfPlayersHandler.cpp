#include "SetTheNumberOfPlayersHandler.h"
#include "UtilityFunctions.h"

#include <cstdint>

SetTheNumberOfPlayersHandler::SetTheNumberOfPlayersHandler(ReadyManager& readyManager)
	: m_readyManager(readyManager)
{
	/* EMPTY */
}

crow::response SetTheNumberOfPlayersHandler::operator()(const crow::request& request) const
{
	auto parsedArguments = ParseUrlArguments(request.body);
	auto end = parsedArguments.end();
	auto sizeIterator = parsedArguments.find("size");

	if (sizeIterator == end) {
		return crow::response(400);
	}
	m_readyManager.SetDesiredNumberOfPlayers(static_cast<uint8_t>(std::stoi(sizeIterator->second)));
	return crow::response(200);
}

uint8_t SetTheNumberOfPlayersHandler::GetNumberOfPlayer() const
{
	return m_readyManager.GetDesiredNumberOfPlayers();
}
