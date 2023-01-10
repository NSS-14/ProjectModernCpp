#pragma once
#include <memory>

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
	const Map& GetMap() const;
	const QuestionManager& GetQuestionManager() const;
	const Question& GetNumericalQuestion() const;
	const Question& GetGridQuestion() const;
	size_t GetNumberOfPlayers() const;
	const std::shared_ptr<Player>& GetPlayerWithName(const std::string& name);
	const std::shared_ptr<Player>& GetPlayerWithColor(const Player::ColorInGame& color);

public:
	void AddUserAndMakeHimPlayer(const User& user);
	//void Start();
	//std::shared_ptr<Player> GiveQuestionToTwo(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2);
	//Ranking GiveNumericalQuestionToAll();

private:
	uint8_t GetRandomPlayerIndex() const;

private:
	//void ChooseBase(Ranking rank);
	//void Update();
	//void FillMap();
	//void StartDuels();

private:
	Map m_map;
	QuestionManager m_qm;
	std::vector<std::shared_ptr<Player>> m_players;
	mutable uint8_t m_phase;
};
