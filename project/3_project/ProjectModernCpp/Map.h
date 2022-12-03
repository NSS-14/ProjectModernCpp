#pragma once
#include<vector>
#include<cstdint>
#include"Region.h"

class Map
{
public:
	using Position = std::pair<uint8_t, uint8_t>;

public:
	Map(size_t height, size_t width);
	Map() = default;

public:
	uint8_t operator [](Position position) const; //getter
	uint8_t& operator [](Position position); //setter
public:
	std::size_t Size();

public:
	friend std::ostream& operator <<(std::ostream& out, const Map& map);

private:
	std::vector<uint8_t> m_regions;
	std::size_t m_height;
	std::size_t m_width;

};

