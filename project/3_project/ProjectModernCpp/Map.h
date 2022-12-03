#pragma once
#include <vector>
#include <cstdint>
#include "Region.h"

class Map
{
public:
	Map(size_t height, size_t width);
	Map() = default;

public:
	uint8_t operator [](Region::Coordinates position) const; //getter
	uint8_t& operator [](Region::Coordinates position); //setter

public:
	std::size_t Size();
	std::vector<Region::Coordinates> Neighbours(Region::Coordinates coordonates);

public:
	friend std::ostream& operator <<(std::ostream& out, const Map& map);

private:
	std::vector<uint8_t> m_regions;
	std::size_t m_height;
	std::size_t m_width;

};

