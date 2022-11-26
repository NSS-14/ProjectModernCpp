#pragma once
#include "Map.h"
#include"QuestionManager.h"
#include"Player.h"

class Game
{
public:
	Game(uint8_t numberOfPlayers);

public:



private:
	Map m_map;
	QuestionManager m_qm;
	std::vector<Player> m_players;
};

