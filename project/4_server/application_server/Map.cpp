#include "Map.h"
#include "PairHash.h"

Map::Map(size_t height, size_t width)
	: m_height(height)
	, m_width(width)
{
	m_regions.resize(height * width, 0);
}

uint8_t Map::operator[](Region::Coordinates position) const
{
	const auto& [line, column] = position;
	return m_regions[line * m_width + column];
}

uint8_t& Map::operator[](Region::Coordinates position)
{
	const auto& [line, column] = position;
	return m_regions[line * m_width + column];
}

std::size_t Map::Size()
{
	return m_regions.size();
}

std::vector<Region::Coordinates> Map::Neighbours(Region::Coordinates coordonates)
{
	std::vector<Region::Coordinates> neighbours;
	Region::Coordinates neighbour;
	neighbour.first = coordonates.first;
	neighbour.second = coordonates.second - 1;

	if (neighbour.second < m_width)
		if ((*this)[coordonates] != (*this)[neighbour])
			neighbours.push_back(neighbour);

	neighbour.first = coordonates.first;
	neighbour.second = coordonates.second + 1;

	if (neighbour.second < m_width)
		if ((*this)[coordonates] != (*this)[neighbour])
			neighbours.push_back(neighbour);

	neighbour.first = coordonates.first - 1;
	neighbour.second = coordonates.second;

	if (neighbour.first < m_height)
		if ((*this)[coordonates] != (*this)[neighbour])
			neighbours.push_back(neighbour);

	neighbour.first = coordonates.first + 1;
	neighbour.second = coordonates.second;

	if (neighbour.first < m_height)
		if ((*this)[coordonates] != (*this)[neighbour])
			neighbours.push_back(neighbour);

	return neighbours;
}

std::vector<Region::Coordinates> Map::Neighbours(uint8_t playerIndex)
{
	std::unordered_set<Region::Coordinates, PairHash::Hash> neighbours;
	Region::Coordinates iterator;
	auto& [line, column] = iterator;
	for (line = 0; line < m_height; ++line)
	{
		for (column = 0; column < m_width; ++column)
		{
			if ((*this)[iterator] == playerIndex)
			{
				std::vector<Region::Coordinates> currentNeighbours;
				currentNeighbours = Neighbours(iterator);
				for (const Region::Coordinates& coordinates : currentNeighbours)
				{
					neighbours.insert(coordinates);
				}
			}
		}
	}
	std::vector<Region::Coordinates > result;
	result.reserve(neighbours.size());
	for (const Region::Coordinates& coordinate : neighbours)
	{
		result.push_back(coordinate);
	}
	return result;
}

std::ostream& operator<<(std::ostream& out, const Map& map)
{
	std::pair<uint8_t, uint8_t> position;
	auto& [line, column] = position;
	for (line = 0; line < map.m_height; line++)
	{
		for (column = 0; column < map.m_width; column++)
		{
			if (map[position])
			{
				out << static_cast <int>(map[position]);
			}
			else
			{
				out << '-';
			}
			out << ' ';
		}
		out << std::endl;
	}
	return out;
}
