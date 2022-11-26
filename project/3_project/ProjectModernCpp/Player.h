#pragma once
#include"Region.h"
#include<unordered_map>

class Player
{
public:
	Player(std::string name, std::string password);
	
	const Region& GetBaseRegion();
	const std::string& GetName() const;
	const std::string& GetPassword() const;

	void SetBaseRegion(const Region& region);
	void SetName(const std::string& name);
	void SetPassword(const std::string& password);

	void InsertRegion(const Region& region);
	Region ExtractRegion(const Region::Coordinates& coordinates);

private:
	Region m_baseRegion;
	std::unordered_map<Region::Coordinates, Region> m_ownedRegions;
	std::string m_name;
	std::string m_password;

};

