#include "Region.h"

Region::Region()
	: m_coordinates()
	, m_score(kInitialScore)
{
	/*EMPTY*/
}

Region::Region(const Coordinates& coordinates, unsigned int score)
	: m_coordinates(coordinates)
	, m_score(score)
{
	/*EMPTY*/
}

bool Region::operator==(const Region& region)
{
	return m_coordinates == region.m_coordinates
		&& m_score == region.m_score;
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
