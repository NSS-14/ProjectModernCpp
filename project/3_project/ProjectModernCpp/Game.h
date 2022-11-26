#pragma once
#include "Map.h"
#include"QuestionManager.h"
#include"Player.h"

class Game
{
public:
	Game(uint8_t numberOfPlayers);

public:
	void AddPlayer(const Player& player);
	void Start();

private:
	uint8_t GetRandomPlayerIndex();

private:
	void GiveNumericalQuestionToAll();
	void ChooseBase();
	void Update();
	void FillMap();

private:
	Map m_map;
	QuestionManager m_qm;
	std::vector<Player> m_players;
};

