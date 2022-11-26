#include "Game.h"

Game::Game(uint8_t numberOfPlayers)
	: m_qm("Questions.txt")

{
	switch (numberOfPlayers)
	{
	case 2:
		m_map = Map(3, 3);
		break;
	case 3:
		m_map = Map(5, 3);
		break;
	case 4:
		m_map = Map(6, 4);
		break;
	}
	m_players.reserve(numberOfPlayers);
}
