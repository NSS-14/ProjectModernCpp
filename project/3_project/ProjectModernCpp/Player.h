#pragma once
#include "Region.h"
#include <unordered_map>
#include <array>
#include <boost/functional/hash.hpp>

class Player
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

	Player(std::string name, std::string password);
	
	const Region& GetBaseRegion();
	const std::string& GetName() const;
	const std::string& GetPassword() const;
	const Region& GetRegion(const Region::Coordinates& coordinates);

	void SetBaseRegion(const Region& region);
	void SetName(const std::string& name);
	void SetPassword(const std::string& password);

	void InsertRegion(const Region& region);
	bool HasRegion(const Region::Coordinates& coordinates);
	Region ExtractRegion(const Region::Coordinates& coordinates);
	
	bool UseAdvantage(uint8_t advantageIndex);

private:
	Region m_baseRegion;
	std::unordered_map<Region::Coordinates, Region, HashPair> m_ownedRegions;
	std::array<bool, 3> m_advantages;
	std::string m_name;
	std::string m_password;

};

