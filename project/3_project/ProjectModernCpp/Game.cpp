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
		m_map = Map(3, 5);
		break;
	case 4:
		m_map = Map(4, 6);
		break;
	}
	m_players.reserve(numberOfPlayers);

	srand(time(0));
}

void Game::Update()
{
	std::cout << m_map;
}

void Game::FillMap()
{
	uint8_t freeSpaces=m_map.Size()-m_players.size();
	Ranking currentRank;
	uint8_t playerToChoose;
	Region::Coordinates inputCoordinate;

	while (freeSpaces)
	{
		currentRank=GiveNumericalQuestionToAll();
		for (uint8_t i = m_players.size() - 1; i > 0; i--)
		{
			playerToChoose = currentRank.Pop();
			for (uint8_t j = 0; j < i; j++)
			{
				std::cout << 'P' << static_cast <int> (playerToChoose) << " choose region: ";
				std::cin >> inputCoordinate.first >> inputCoordinate.second;
				inputCoordinate.first -= 48;
				inputCoordinate.second -= 48;

				m_map[inputCoordinate] = playerToChoose + 1;
				m_players[playerToChoose].SetBaseRegion(Region(inputCoordinate));
				freeSpaces--;
			}
			Update();
		}
		
	}
	
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
	Ranking currentrank;
	currentrank = GiveNumericalQuestionToAll();

	Update();
	ChooseBase(currentrank);
	Update();
	FillMap();
	Update();
}

uint8_t Game::GiveQuestionToTwo(uint8_t player1, uint8_t player2)
{
	std::vector<uint8_t> duelPlayers = {player1, player2};

	Ranking currentRanking;
	Question question;
	float correctAnswer;
	std::string correctAnswerString;

	question = m_qm.GetGridQuestion();
	correctAnswerString = question.GetAnswer();

	std::cout << question.GetQuestion();
	std::cout << std::endl;

	std::vector<std::string> answersFromPlayers;
	std::string answerFromPlayer;

	for (int i = 0; i < 2; ++i)
	{
		std::cout << 'P' << static_cast<int>(duelPlayers[i]) << ": ";
		
		std::cin >> answerFromPlayer;
		answersFromPlayers.push_back(answerFromPlayer);
	}

	if (answersFromPlayers[0] == answersFromPlayers[1]) {
		if (answersFromPlayers[0] == correctAnswerString) {
			question = m_qm.GetNumericalQuestion();
			correctAnswerString = question.GetAnswer();
			correctAnswer = std::stof(correctAnswerString);

			std::cout << question.GetQuestion();
			std::cout << std::endl;

			float floatAnswerFromPlayer;
			for (int i = 0; i < 2; ++i)
			{
				std::cout << 'P' << static_cast<int>(duelPlayers[i]) << ": ";
				time_t beforeQuestionTime = time(0);
				std::cin >> floatAnswerFromPlayer;
				currentRanking.Push
				(
					i,
					std::abs(correctAnswer - floatAnswerFromPlayer),
					time(0) - beforeQuestionTime
				);
			}

			return currentRanking.Pop();
		}

		return m_players.size() + 1;
	}

	if (answersFromPlayers[0] == correctAnswerString)
		return duelPlayers[0];
	else if (answersFromPlayers[1] == correctAnswerString)
		return duelPlayers[1];

	return m_players.size() + 1;
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
		std::cout << 'P' << static_cast<int>(playerToChoose) << ':' << std::endl;
		std::cin >> input.first;
		std::cin >> input.second;
		input.first -= 48;
		input.second -= 48;

		m_map[input] =playerToChoose+1;
		m_players[playerToChoose].SetBaseRegion(Region(input,kBaseScore));
	}
}

uint8_t Game::GetRandomPlayerIndex()
{
	return rand() % m_players.size();
}
