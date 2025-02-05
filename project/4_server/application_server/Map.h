#pragma once
#include <cstdint>
#include <tuple>
#include <iostream>
#include <vector>
#include "Player.h"
#include <optional>

class Map
{
public:
	using Coordinates = std::pair<uint8_t, uint8_t>;

public:
	Map() = default;
	Map(size_t height, size_t width);
	Map(const Map& map);

public:
	Map& operator =(const Map& map);
	bool operator ==(const Map& map);
	const std::shared_ptr<Player>& operator [](const Coordinates& position) const; //getter
	std::shared_ptr<Player>& operator [](const Coordinates& position); //setter
	friend std::ostream& operator <<(std::ostream& out, const Map& map);

public:
	std::size_t GetHeight() const;
	std::size_t GetWidth() const;

public:
	void SetHeight(std::size_t height);
	void SetWidth(std::size_t width);

public:
	std::size_t Size() const;
	std::string ToString() const;
	std::vector<Coordinates> Neighbours(const Coordinates& coordinates) const;
	std::vector<Coordinates> Neighbours(std::shared_ptr<Player> player) const; // to reimplement in such manner that it will search in players region only
	size_t FreeSpacesLeft() const;

private:
	std::vector<std::shared_ptr<Player>> m_playersOnMap;
	std::size_t m_height;
	std::size_t m_width;
};
