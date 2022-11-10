#include "Region.h"

Region::Region()
{
	m_score = 100;
}

Region::Region(unsigned int score)
{
	m_score = score;
}

unsigned Region::getScore() const
{
	return m_score;
}

void Region::IncrementScore()
{
	m_score += 100;
}

void Region::DecrementScore()
{
	if (m_score == 100)
		return;
	m_score -= 100;
}
