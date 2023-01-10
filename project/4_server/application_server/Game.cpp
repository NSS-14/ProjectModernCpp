#include "Game.h"
#include "StorageManager.h"

#include <unordered_set>

Game::Game(uint8_t numberOfPlayers)
	: m_phase(0)
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
	m_qm.ReadDataBase(StorageManager::GetInstance().GetStorage());
	srand(time(0));
}

const Map& Game::GetMap() const
{
	return m_map;
}
const QuestionManager& Game::GetQuestionManager() const
{
	return m_qm;
}
const Question& Game::GetNumericalQuestion() const
{
	return m_qm.GetNumericalQuestion();
}
const Question& Game::GetGridQuestion() const
{
	return m_qm.GetGridQuestion();
}
size_t Game::GetNumberOfPlayers() const
{
	return m_players.size();
}

//void Game::Update()
//{
//	std::cout << m_map;
//}

//void Game::FillMap()
//{
//	uint8_t freeSpaces = m_map.Size() - m_players.size();
//	Ranking currentRank;
//	std::shared_ptr<Player> playerToChoose;
//	Player::Coordinates inputCoordinate;
//
//	while (freeSpaces)
//	{
//		currentRank = GiveNumericalQuestionToAll();
//		for (uint8_t i = m_players.size() - 1; i > 0; i--)
//		{
//			playerToChoose = currentRank.Pop();
//			for (uint8_t j = 0; j < i; j++)
//			{
//				std::cout << 'P' << playerToChoose->GetName() << " choose region: ";
//				std::cin >> inputCoordinate.first >> inputCoordinate.second;
//				inputCoordinate.first -= 48;
//				inputCoordinate.second -= 48;
//
//				m_map[inputCoordinate] = playerToChoose;
//				playerToChoose->InsertRegion({ inputCoordinate, Player::kScoreDefaultValue });
//				freeSpaces--;
//			}
//			Update();
//		}
//	}
//}

//void Game::StartDuels()
//{
//	for (int i = 0; i < m_players.size() + 1; ++i) {
//		std::vector<Player::Coordinates> neighbours;
//		std::vector<uint8_t> randomPlayerOrder;
//		randomPlayerOrder.reserve(m_players.size());
//		std::unordered_set<uint8_t> uSet;
//
//		while (uSet.size() != m_players.size()) {
//			uSet.insert(GetRandomPlayerIndex());
//		}
//
//		for (uint8_t index : uSet) {
//			randomPlayerOrder.push_back(index);
//		}
//
//		std::shared_ptr<Player> duelPlayerIndex, winner;
//
//		for (int j = 0; j < randomPlayerOrder.size(); ++j) {
//			std::vector<Player::Coordinates> neighbours = m_map.Neighbours(std::make_shared<Player>(m_players[randomPlayerOrder[j]]));
//			for (int k = 0; k < neighbours.size(); ++k)
//			{
//				std::cout << "(" << static_cast<int>(neighbours[k].first) << "," << static_cast<int>(neighbours[k].second) << ")" << ";";
//			}
//			std::cout << std::endl;
//			std::cout << 'P' << static_cast<int>(randomPlayerOrder[j] + 1) << "chooses duel with: ";
//
//			Player::Coordinates duelRegion;
//
//			std::cin >> duelRegion.first >> duelRegion.second;
//			duelRegion.first -= 48;
//			duelRegion.second -= 48;
//			winner = GiveQuestionToTwo(std::make_shared<Player>(randomPlayerOrder[j]), m_map[duelRegion]);
//			if (*winner == m_players[randomPlayerOrder[j]])
//			{
//				if (m_map[duelRegion]->GetScore(duelRegion) == 100)
//				{
//					m_players[randomPlayerOrder[j]].InsertRegion(m_map[duelRegion]->ExtractRegion(duelRegion));
//					m_map[duelRegion] = std::make_shared<Player>(m_players[randomPlayerOrder[j]]);
//				}
//				else
//				{
//					m_map[duelRegion]->DecrementScore(duelRegion);
//				}
//			}
//			if (!winner)
//			{
//				continue;
//			}
//			m_map[duelRegion]->IncrementScore(duelRegion);
//			Update();
//		}
//	}
//}

void Game::AddUserAndMakeHimPlayer(const User& user)
{
	Player toBeAdded(user);
	switch (m_players.size())
	{
	case 0: toBeAdded.SetColorInGame(Player::ColorInGame::Blue); break;
	case 1:	toBeAdded.SetColorInGame(Player::ColorInGame::Yellow); break;
	case 2:	toBeAdded.SetColorInGame(Player::ColorInGame::Red); break;
	case 3:	toBeAdded.SetColorInGame(Player::ColorInGame::Green); break;
	}
	m_players.push_back(std::make_shared<Player>(std::move(toBeAdded)));
}

//void Game::Start()
//{
//	Ranking currentrank;
//	currentrank = GiveNumericalQuestionToAll();
//
//	Update();
//	ChooseBase(currentrank);
//	Update();
//	FillMap();
//	StartDuels();
//	Update();
//}

//std::shared_ptr<Player> Game::GiveQuestionToTwo(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
//{
//	std::vector<std::shared_ptr<Player>> duelPlayers = { player1, player2 };
//
//	Ranking currentRanking;
//	Question question;
//	float correctAnswer;
//	std::string correctAnswerString;
//
//	question = m_qm.GetGridQuestion();
//	correctAnswerString = question.GetAnswer();
//
//	std::cout << question.GetQuestion();
//	std::cout << std::endl;
//
//	std::vector<std::string> answersFromPlayers;
//	std::string answerFromPlayer;
//
//	std::cin.get();
//	for (int i = 0; i < 2; ++i)
//	{
//		std::vector<std::string> variants = question.GetAnswers();
//		for (int j = 0; j < variants.size(); ++j)
//		{
//			std::cout << j + 1 << ". " << variants[j];
//		}
//		std::cout << 'P' << duelPlayers[i]->GetName() << ": ";
//		std::getline(std::cin, answerFromPlayer);
//		answersFromPlayers.push_back(answerFromPlayer);
//	}
//
//	if (answersFromPlayers[0] == answersFromPlayers[1]) {
//		if (answersFromPlayers[0] == correctAnswerString) {
//			question = m_qm.GetNumericalQuestion();
//			correctAnswerString = question.GetAnswer();
//			correctAnswer = std::stof(correctAnswerString);
//
//			std::cout << question.GetQuestion();
//			std::cout << std::endl;
//
//			float floatAnswerFromPlayer;
//			for (int i = 0; i < 2; ++i)
//			{
//				std::cout << 'P' << duelPlayers[i]->GetName() << ": ";
//				time_t beforeQuestionTime = time(0);
//				std::cin >> floatAnswerFromPlayer;
//				currentRanking.Push
//				(
//					duelPlayers[i],
//					std::abs(correctAnswer - floatAnswerFromPlayer),
//					time(0) - beforeQuestionTime
//				);
//			}
//
//			return currentRanking.Pop();
//		}
//
//		return {};
//	}
//
//	if (answersFromPlayers[0] == correctAnswerString)
//		return duelPlayers[0];
//	else if (answersFromPlayers[1] == correctAnswerString)
//		return duelPlayers[1];
//
//	return {};
//}

//Ranking Game::GiveNumericalQuestionToAll()
//{
//	Ranking currentRanking;
//	Question numericalQuestion;
//	float correctAnswer;
//	std::string correctAnswerString;
//
//	numericalQuestion = m_qm.GetNumericalQuestion();
//	correctAnswerString = numericalQuestion.GetAnswer();
//	correctAnswer = std::stof(correctAnswerString);
//
//	std::cout << numericalQuestion.GetQuestion();
//	std::cout << std::endl;
//
//	float answerFromPlayer;
//	for (int i = 0; i < m_players.size(); ++i)
//	{
//		std::cout << "P" << i << ": ";
//		time_t beforeQuestionTime = time(0);
//		std::cin >> answerFromPlayer;
//		currentRanking.Push
//		(
//			std::make_shared<Player>(m_players[i]),
//			std::abs(correctAnswer - answerFromPlayer),
//			time(0) - beforeQuestionTime
//		);
//	}
//
//	return currentRanking;
//}

//void Game::ChooseBase(Ranking rank)
//{
//	std::shared_ptr<Player> playerToChoose;
//	std::cout << std::endl;
//	std::cout << "Choose base phase:" << std::endl;
//	Player::Coordinates input;
//	for (int i = 0; i < m_players.size(); ++i)
//	{
//		playerToChoose = rank.Pop();
//		std::cout << 'P' << playerToChoose->GetName() << ':' << std::endl;
//		std::cin >> input.first;
//		std::cin >> input.second;
//		input.first -= 48;
//		input.second -= 48;
//
//		m_map[input] = playerToChoose;
//		playerToChoose->AddNewRegionAt(input);
//	}
//}

uint8_t Game::GetRandomPlayerIndex() const
{
	return rand() % m_players.size();
}
