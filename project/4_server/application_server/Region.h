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
	Region(const Region& region);
	Region(const Coordinates& coordinates, unsigned int score = kInitialScore);

public:
	Region& operator =(const Region& region);
	bool operator ==(const Region& region);
	friend std::ostream& operator <<(std::ostream& out, const Region& region);

public:
	unsigned int GetScore() const;
	const Coordinates& GetCoordinates() const;

public:
	void SetScore(unsigned int score);
	void SetCoordinates(const Coordinates& coordinates);

public:
	void IncrementScore();
	void DecrementScore();

private:
	unsigned int m_score;
	Coordinates m_coordinates;
};

