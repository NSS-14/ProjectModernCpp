#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <unordered_set>

class Map
{
public:
	using Coordinates = std::pair<uint8_t, uint8_t>;

public:
	Map(size_t height, size_t width);
	Map() = default;
	Map(const Map& map);

public:
	Map& operator =(const Map& map);
	bool operator ==(const Map& map);
	uint8_t operator [](const Coordinates& position) const; //getter
	uint8_t& operator [](const Coordinates& position); //setter

public:
	std::size_t GetHeight() const;
	std::size_t GetWidth() const;

public:
	void SetHeight(std::size_t height);
	void SetWidth(std::size_t width);

public:
	std::size_t Size();
	std::vector<Coordinates> Neighbours(const Coordinates& coordinates);
	std::vector<Coordinates> Neighbours(uint8_t playerIndex);

public:
	friend std::ostream& operator <<(std::ostream& out, const Map& map);

private:
	std::vector<uint8_t> m_players;//to rename or use reference of players
	std::size_t m_height;
	std::size_t m_width;

};

