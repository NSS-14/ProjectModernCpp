#pragma once

#include <unordered_map>
#include <array>

#include "User.h"
#include "PairHashers.h"

class Player : public User
{
public:
	enum class Advantage : uint8_t
	{
		FiftyFifty, // Grid questions
		ChooseAnswer, // Numerical questions
		Suggestion, // Numerical questions
		NumberOfAdvantages,
		UsedAdvantage
	};
	enum class ColorInGame : uint8_t
	{
		Blue,
		Yellow,
		Red,
		Green,
		NumberOfColors,
		UninitializedColor
	};

public:
	static inline const size_t kNumberOfAdvantages = static_cast<size_t>(Advantage::NumberOfAdvantages);
	static inline const unsigned int kScoreStepValue = 100;
	static inline const unsigned int kScoreDefaultValue = 100;
	static inline const unsigned int kScoreBaseRegionDefaultValue = 300;

public:
	using Coordinates = std::pair<uint8_t, uint8_t>;
	using Score = unsigned int;
	using Region = std::pair<Coordinates, Score>;
	using MapOfRegions = std::unordered_map<Coordinates, Score, PairHashers::HashStringableTypes>;
	using AdvantageArray = std::array<Advantage, kNumberOfAdvantages>;

public:
	Player();
	Player(unsigned int id, std::string name, std::string password, const ColorInGame& colorInGame = ColorInGame::UninitializedColor);
	Player(const Player& player);
	Player(Player&& player) noexcept;
	Player(const User& user);
	Player(User&& user) noexcept;

public:
	Player& operator =(const Player& player);
	Player& operator =(Player&& player) noexcept;
	bool operator ==(const Player& player);

public:
	unsigned int GetScore() const;
	unsigned int GetScore(const Coordinates& coordinates) const; // this returns the score of the region from the coordinates
	const AdvantageArray& GetAdvantages() const;
	const ColorInGame& GetColorInGame() const;

public:
	void SetColorInGame(const ColorInGame& colorInGame);

public:
	void IncrementScore(const Coordinates& coordinates);
	void DecrementScore(const Coordinates& coordinates);

public:
	void AddNewRegionAt(const Coordinates& coordinates);
	void InsertRegion(Region region);
	Region ExtractRegion(const Coordinates& coordinates);
	bool HasRegionOn(const Coordinates& coordinates);
	bool UseAdvantage(Advantage advantage);
	bool DoIHaveARegionWithScoreGreatherThan(unsigned int gratherThan);

private:
	void InitializeAdvantages();

private:
	MapOfRegions m_ownedRegions;
	AdvantageArray m_advantages;
	ColorInGame m_colorInGame;
};

bool operator ==(const Player::MapOfRegions& firstMapOfRegions, const Player::MapOfRegions& secondMapOfRegions);
std::string CastToStringAdvantage(const Player::Advantage& advantage);
