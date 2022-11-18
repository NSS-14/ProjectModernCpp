#include "Region.h"

Region::Region()
{
	m_score = kInitialScore;
	m_coordinates = Coordinates(200, 200);
}

Region::Region(unsigned int score, const Coordinates& coordinates)
{
	m_score = score;
	m_coordinates = coordinates;
}

unsigned Region::getScore() const
{
	return m_score;
}

Region::Coordinates Region::getCoordinates() const
{
	return m_coordinates;
}

void Region::IncrementScore()
{
	m_score += kStepValue;
}

void Region::DecrementScore()
{
	if (m_score == kInitialScore)
		return;
	m_score -= kStepValue;
}

std::ostream& operator<<(std::ostream& out, const Region& region)
{
	return out << region.m_score;
}
