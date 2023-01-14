#include "Player.h"

Player::Player()
	: User()
	, m_advantages()
	, m_ownedRegions()
	, m_colorInGame(ColorInGame::UninitializedColor)
{
	InitializeAdvantages();
}
Player::Player(unsigned int id, std::string name, std::string password, const ColorInGame& colorInGame)
	: User(id, name, password)
	, m_ownedRegions()
	, m_advantages()
	, m_colorInGame(colorInGame)
{
	InitializeAdvantages();
}
Player::Player(const Player& player)
{
	*this = player;
}
Player::Player(Player&& player) noexcept
{
	*this = std::move(player);
}
Player::Player(const User& user)
	: User(user)
	, m_advantages()
	, m_ownedRegions()
{
	InitializeAdvantages();
}
Player::Player(User&& user) noexcept
	: User(std::move(user))
	, m_advantages()
	, m_ownedRegions()
{
	InitializeAdvantages();
}

Player& Player::operator=(const Player& player)
{
	m_ownedRegions = player.m_ownedRegions;
	m_advantages = player.m_advantages;
	m_id = player.m_id;
	m_name = player.m_name;
	m_password = player.m_password;
	m_colorInGame = player.m_colorInGame;
	return *this;
}
Player& Player::operator=(Player&& player) noexcept
{
	m_ownedRegions = std::move(player.m_ownedRegions);
	m_advantages = std::move(player.m_advantages);
	m_id = player.m_id;
	m_name = std::move(player.m_name);
	m_password = std::move(player.m_password);
	m_colorInGame = player.m_colorInGame;
	return *this;
}
bool Player::operator==(const Player& player)
{
	return m_ownedRegions == player.m_ownedRegions
		&& m_advantages == player.m_advantages
		&& m_id == player.m_id
		&& m_name == player.m_name
		&& m_password == player.m_password
		&& m_colorInGame == player.m_colorInGame;
}

unsigned int Player::GetScore() const
{
	unsigned int totalScore = 0;
	for (const auto& element : m_ownedRegions)
	{
		totalScore += element.second;
	}
	return totalScore;
}

unsigned int Player::GetScore(const Coordinates& coordinates) const
{
	return m_ownedRegions.at(coordinates);
}
const Player::AdvantageArray& Player::GetAdvantages() const
{
	return m_advantages;
}
const Player::ColorInGame& Player::GetColorInGame() const
{
	return m_colorInGame;
}

void Player::SetColorInGame(const ColorInGame& colorInGame)
{
	m_colorInGame = colorInGame;
}

void Player::IncrementScore(const Coordinates& coordinates)
{
	if (!m_ownedRegions.count(coordinates)) {
		return;
	}
	m_ownedRegions[coordinates] += kScoreStepValue;
}
void Player::DecrementScore(const Coordinates& coordinates)
{
	if (!m_ownedRegions.count(coordinates)) {
		return;
	}
	if (m_ownedRegions[coordinates] == kScoreDefaultValue) {
		return;
	}
	m_ownedRegions[coordinates] -= kScoreStepValue;
}
void Player::AddNewRegionAt(const Coordinates& coordinates)
{
	if (m_ownedRegions.count(coordinates)) {
		return;
	}
	if (!m_ownedRegions.size()) {
		m_ownedRegions.emplace(coordinates, kScoreBaseRegionDefaultValue);
		return;
	}
	m_ownedRegions.emplace(coordinates, kScoreDefaultValue);
}

void Player::InsertRegion(Region region)
{
	m_ownedRegions[region.first] = region.second;
}
Player::Region Player::ExtractRegion(const Coordinates& coordinates)
{
	Region result;
	result.first = coordinates;
	result.second = m_ownedRegions[coordinates];
	m_ownedRegions.erase(coordinates);
	return result;
	/*auto region = m_ownedRegions.extract(coordinates);
	if (region)
		return { std::move(region.key()), std::move(region.mapped()) };
	throw "Region not found.";*/
}
bool Player::HasRegionOn(const Coordinates& coordinates)
{
	if (m_ownedRegions.count(coordinates))
		return true;
	return false;
}
bool Player::UseAdvantage(Advantage advantage)
{
	if (m_advantages[static_cast<size_t>(advantage)] == Advantage::UsedAdvantage) {
		return false;
	}
	if (!DoIHaveARegionWithScoreGreatherThan(200)) {
		return false;
	}
	m_advantages[static_cast<size_t>(advantage)] = Advantage::UsedAdvantage;
	return true;
}
bool Player::DoIHaveARegionWithScoreGreatherThan(unsigned int gratherThan)
{
	for (const Region& region : m_ownedRegions) {
		if (region.second >= gratherThan) {
			return true;
		}
	}
	return false;
}

void Player::InitializeAdvantages()
{
	for (uint8_t i = 0; i < kNumberOfAdvantages; ++i) {
		m_advantages[i] = static_cast<Advantage>(i);
	}
}

bool operator==(const Player::MapOfRegions& firstMapOfRegions, const Player::MapOfRegions& secondMapOfRegions)
{
	if (firstMapOfRegions.size() != secondMapOfRegions.size()) {
		return false;
	}
	Player::MapOfRegions::const_iterator firstMapIt;
	Player::MapOfRegions::const_iterator secondMapIt;
	for (firstMapIt = firstMapOfRegions.begin(), secondMapIt = secondMapOfRegions.begin();
		firstMapIt != firstMapOfRegions.end() && secondMapIt != secondMapOfRegions.end();
		++firstMapIt, ++secondMapIt)
	{
		if (firstMapIt->first != secondMapIt->first ||
			firstMapIt->second != secondMapIt->second) {
			return false;
		}
	}
	return true;
}
std::string CastToStringAdvantage(const Player::Advantage& advantage)
{
	switch (advantage)
	{
	case(Player::Advantage::FiftyFifty): return "FiftyFifty";
	case(Player::Advantage::ChooseAnswer): return "ChooseAnswer";
	case(Player::Advantage::Suggestion): return "Suggestion";
	default: return "UsedAdvantage";
	}
}
