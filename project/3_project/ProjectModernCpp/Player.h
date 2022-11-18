#pragma once
#include"Region.h"
#include<unordered_map>

class Player
{
public:
	Player(std::string name, std::string password);
	
	Region GetBase();
	void SetBase(const Region& region);

	const std::string& GetName() const;
	void SetName(std::string& name);

	const std::string& GetPassword() const;
	void SetPassword(std::string& password);

	void InsertRegion(const Region& region);
	Region ExtractRegion(Region::Coordinates coordinates);

private:
	Region m_baseRegion;
	std::unordered_map<Region::Coordinates, Region> m_ownedRegions;
	std::string m_name;
	std::string m_password;

};

