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

	srand(time(0));
}

void Game::Update()
{
	std::cout << m_map;
}

void Game::AddPlayer(const Player& player)
{
	m_players.push_back(player);
}

void Game::Start()
{
	/*
	if (m_players.capacity() == m_players.size())
	{
		return;
	}
	*/
	Ranking currentRank;
	currentRank = GiveNumericalQuestionToAll();

	Update();
	ChooseBase(currentRank);
	Update();
}

Ranking Game::GiveNumericalQuestionToAll()
{
	Ranking currentRanking;
	Question numericalQuestion;
	float correctAnswer;
	std::string correctAnswerString;

	numericalQuestion = m_qm.GetNumericalQuestion();
	correctAnswerString = numericalQuestion.GetAnswer();
	correctAnswer = std::stof(correctAnswerString);

	std::cout << numericalQuestion.GetQuestion();
	std::cout << std::endl;

	float answerFromPlayer;
	for (int i = 0; i < m_players.size(); ++i)
	{
		std::cout << "P" << i << ": ";
		time_t beforeQuestionTime = time(0);
		std::cin >> answerFromPlayer;
		currentRanking.Push
		(
			i,
			std::abs(correctAnswer - answerFromPlayer),
			time(0) - beforeQuestionTime
		);
	}

	return currentRanking;
}

void Game::ChooseBase(Ranking rank)
{
	uint8_t playerToChoose;
	std::cout << std::endl;
	std::cout << "Choose base phase:" << std::endl;
	Region::Coordinates input;
	for (int i = 0; i < m_players.size(); ++i)
	{
		playerToChoose = rank.Pop();
		std::cout << 'P' << playerToChoose << ':' << std::endl;
		std::cin >> input.first;
		std::cin >> input.second;

		m_map[input] = Region(300, input);
		m_players[playerToChoose].SetBaseRegion(*m_map[input]);
	}
}

uint8_t Game::GetRandomPlayerIndex()
{
	return rand() % m_players.size();
}
