#pragma once
#include "Region.h"
#include <unordered_map>
#include <array>
#include"User.h"
#include <boost/functional/hash.hpp>

class Player : public User
{
public:
	struct HashPair {
		template<class T1, class T2> 
		size_t operator()(const std::pair<T1, T2> pair) const {
			auto hash1 = boost::hash<T1>()(pair.first);
			auto hash2 = boost::hash<T2>()(pair.second);
			
			if (hash1 != hash2)
				return hash1 ^ hash2;
			return hash1;
		}
	};
	Player();
	Player(std::string name, std::string password);
	Player(const Player& player);
	Player(const User& user);
	
public:
	Player& operator =(const Player& player);
	bool operator ==(const Player& player);

public:
	const Region& GetBaseRegion();
	const Region& GetRegion(const Region::Coordinates& coordinates) const;
	unsigned int GetScore() const;

public:
	void SetBaseRegion(const Region& region);
	Region& GetRegion(const Region::Coordinates& coordinates);

public:
	void InsertRegion(const Region& region);
	bool HasRegion(const Region::Coordinates& coordinates);
	Region ExtractRegion(const Region::Coordinates& coordinates);
	
	bool UseAdvantage(uint8_t advantageIndex);

private:
	Region m_baseRegion;
	std::unordered_map<Region::Coordinates, Region, HashPair> m_ownedRegions;
	std::array<bool, 3> m_advantages;

};

