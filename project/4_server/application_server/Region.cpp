#include "Region.h"

Region::Region()
	: m_coordinates()
	, m_score(kInitialScore)
{
	/*EMPTY*/
}

Region::Region(const Region& region)
{
	*this = region;
}

Region::Region(const Coordinates& coordinates, unsigned int score)
	: m_coordinates(coordinates)
	, m_score(score)
{
	/*EMPTY*/
}

Region& Region::operator=(const Region& region)
{
	m_coordinates = region.m_coordinates;
	m_score = region.m_score;
	return *this;
}

bool Region::operator==(const Region& region)
{
	return m_coordinates == region.m_coordinates
		&& m_score == region.m_score;
}

unsigned Region::GetScore() const
{
	return m_score;
}

const Region::Coordinates& Region::GetCoordinates() const
{
	return m_coordinates;
}

void Region::SetScore(unsigned int score)
{
	m_score = score;
}

void Region::SetCoordinates(const Coordinates& coordinates)
{
	m_coordinates = coordinates;
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
