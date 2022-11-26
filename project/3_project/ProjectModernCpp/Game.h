#pragma once
#include "Map.h"
#include "QuestionManager.h"
#include "Player.h"
#include "Ranking.h"

class Game
{
public:
	Game(uint8_t numberOfPlayers);

public:
	void AddPlayer(const Player& player);
	void Start();
	Ranking GiveNumericalQuestionToAll();
	void ChooseBase(const Ranking& rank);

private:
	uint8_t GetRandomPlayerIndex();

private:
	void ChooseBase();
	void Update();
	void FillMap();


private:
	Map m_map;
	QuestionManager m_qm;
	std::vector<Player> m_players;
};

