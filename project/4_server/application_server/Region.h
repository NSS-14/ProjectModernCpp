#pragma once

#include <iostream>
#include <cstdint>

class Region
{
public:
	using Coordinates = std::pair<uint8_t, uint8_t>;

public:
	static const unsigned int kInitialScore = 100;
	static const unsigned int kStepValue = 100;

public:
	Region();
	Region(const Coordinates& coordinates, unsigned int score = kInitialScore);

public:
	bool operator ==(const Region& region);
	friend std::ostream& operator <<(std::ostream& out, const Region& region);

public:
	unsigned getScore() const;
	Coordinates getCoordinates() const;

public:
	void IncrementScore();
	void DecrementScore();

private:
	unsigned int m_score;
	Coordinates m_coordinates;
};

