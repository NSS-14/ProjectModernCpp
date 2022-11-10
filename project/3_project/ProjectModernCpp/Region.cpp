#include "Region.h"

Region::Region()
{
	m_score = kInitialScore;
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
	m_score += kStepValue;
}

void Region::DecrementScore()
{
	if (m_score == kInitialScore)
		return;
	m_score -= kStepValue;
}
