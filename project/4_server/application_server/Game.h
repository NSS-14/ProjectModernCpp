#pragma once
#include "Map.h"
#include "QuestionManager.h"
#include "Player.h"
#include "Ranking.h" 

class Game
{
public:
	static const unsigned int kBaseScore = 300;

public:
	Game(uint8_t numberOfPlayers);

public:
	void AddPlayer(const Player& player);
	void Start();
	std::shared_ptr<Player> GiveQuestionToTwo(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2);
	Ranking GiveNumericalQuestionToAll();

private:
	uint8_t GetRandomPlayerIndex();

private:
	void ChooseBase(Ranking rank);
	void Update();
	void FillMap();
	void StartDuels();

private:
	Map m_map;
	QuestionManager m_qm;
	std::vector<Player> m_players;
};