#pragma once
#include <vector>
#include <cstdint>
#include "Region.h"
#include <unordered_set>

class Map
{
public:
	Map(size_t height, size_t width);
	Map() = default;
	Map(const Map& map);

public:
	Map& operator =(const Map& map);
	bool operator ==(const Map& map);
	uint8_t operator [](Region::Coordinates position) const; //getter
	uint8_t& operator [](Region::Coordinates position); //setter

public:
	std::size_t GetHeight() const;
	std::size_t GetWidth() const;

public:
	void SetHeight(std::size_t height);
	void SetWidth(std::size_t width);

public:
	std::size_t Size();
	std::vector<Region::Coordinates> Neighbours(Region::Coordinates coordonates);
	std::vector<Region::Coordinates> Neighbours(uint8_t playerIndex);
public:
	friend std::ostream& operator <<(std::ostream& out, const Map& map);

private:
	std::vector<uint8_t> m_regions;//to rename
	std::size_t m_height;
	std::size_t m_width;

};

