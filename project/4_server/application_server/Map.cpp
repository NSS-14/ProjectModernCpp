#include "Map.h"
Map::Map(size_t height, size_t width)
	: m_height(height)
	, m_width(width)
{
	m_playersOnMap.resize(height * width);
}
Map::Map(const Map& map)
{
	*this = map;
}

Map& Map::operator=(const Map& map)
{
	m_playersOnMap = map.m_playersOnMap;
	m_height = map.m_height;
	m_width = map.m_width;
	return *this;
}
bool Map::operator==(const Map& map)
{
	return m_playersOnMap == map.m_playersOnMap
		&& m_height == map.m_height
		&& m_width == map.m_width;
}
const std::shared_ptr<Player>& Map::operator[](const Coordinates& position) const
{
	const auto& [line, column] = position;
	return m_playersOnMap[line * m_width + column];
}
std::shared_ptr<Player>& Map::operator[](const Coordinates& position)
{
	const auto& [line, column] = position;
	return m_playersOnMap[line * m_width + column];
}

std::size_t Map::GetHeight() const
{
	return m_height;
}
std::size_t Map::GetWidth() const
{
	return m_width;
}

void Map::SetHeight(std::size_t height)
{
	m_height = height;
}
void Map::SetWidth(std::size_t width)
{
	m_width = width;
}

std::size_t Map::Size()
{
	return m_playersOnMap.size();
}

std::ostream& operator<<(std::ostream& out, const Map& map)
{
	std::pair<uint8_t, uint8_t> position;
	auto& [line, column] = position;
	for (line = 0; line < map.m_height; line++)
	{
		for (column = 0; column < map.m_width; column++)
		{
			if (map[position].get() != nullptr)
			{
				out << map[position]->GetName();
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