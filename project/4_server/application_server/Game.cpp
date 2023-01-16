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
const std::shared_ptr<Player>& Game::GetPlayerWithName(const std::string& name)
{
	for (const std::shared_ptr<Player>& sharedPlayer : m_players) {
		if (sharedPlayer->GetName() == name) {
			return sharedPlayer;
		}
	}
	return {};
}
const std::shared_ptr<Player>& Game::GetPlayerWithColor(const Player::ColorInGame& color)
{
	for (const std::shared_ptr<Player>& sharedPlayer : m_players) {
		if (sharedPlayer->GetColorInGame() == color) {
			return sharedPlayer;
		}
	}
	return {};
}
std::vector<std::shared_ptr<Player>> Game::GetPlayersInRandomOrganizedOrder() const
{
	std::vector<std::shared_ptr<Player>> result;
	std::unordered_set<uint8_t> alreadyPlacedPlayersIndexes;
	while (true) {
		uint8_t nextPlayerIndex = GetRandomPlayerIndex();
		if (alreadyPlacedPlayersIndexes.count(nextPlayerIndex)) {
			continue;
		}
		alreadyPlacedPlayersIndexes.insert(nextPlayerIndex);
		result.push_back(m_players[nextPlayerIndex]);
		if (result.size() == GetNumberOfPlayers()) {
			break;
		}
	}
	return result;
}
std::string Game::GetGameResult()
{
	std::string result;
	int max = 0;
	int index;
	for (size_t i = 0; i < m_players.size(); ++i) {
		int score = m_players[i]->GetScore();
		if (max < score) {
			index = i;
			max = score;
		}
		result += m_players[i]->GetName() + "-" + std::to_string(score) + " ";
	}
	result += "\n";
	std::vector<int> draw;
	for (size_t i = 0; i < m_players.size(); ++i) {
		int score = m_players[i]->GetScore();
		if (max == score) {
			draw.push_back(i);
		}
	}
	if (draw.size() == 1) {
		result += "The winner is: " + m_players[index]->GetName();
	}
	else {
		result += "We have draw between some players: ";
		for (int drawIndex : draw) {
			result += m_players[drawIndex]->GetName() + " ";
		}
	}
	return result;
}
Map& Game::GetMap()
{
	return m_map;
}
uint8_t Game::GetRandomPlayerIndex() const
{
	return rand() % m_players.size();
}

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
