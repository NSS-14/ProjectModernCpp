#include "Map.h"

Map::Map(size_t height, size_t width)
	: m_height(height)
	, m_width(width)
{
	m_regions.resize(height * width, 0);
}

uint8_t Map::operator[](Position position) const
{
	const auto& [line, column] = position;
	return m_regions[line * m_width + column];
}

uint8_t& Map::operator[](Position position)
{
	const auto& [line, column] = position;
	return m_regions[line * m_width + column];
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
