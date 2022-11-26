#pragma once
#include<vector>
#include<cstdint>
#include<optional>
#include"Region.h"

class Map
{
public:
	using Position = std::pair<uint8_t, uint8_t>;

public:
	Map(size_t height, size_t width);
	Map() = default;

public:
	const std::optional<Region>& operator [](Position position) const; //getter
	std::optional<Region>& operator [](Position position); //setter

public:
	friend std::ostream& operator <<(std::ostream& out, const Map& map);

private:
	std::vector<std::optional<Region>> m_regions;
	std::size_t m_height;
	std::size_t m_width;

};

