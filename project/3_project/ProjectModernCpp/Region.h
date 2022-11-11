#pragma once

#include <iostream>

class Region
{
public:
	static const unsigned int kInitialScore = 100;
	static const unsigned int kStepValue = 100;

public:
	Region();
	Region(unsigned int score);

public:
	unsigned getScore() const;

public:
	void IncrementScore();
	void DecrementScore();
	friend std::ostream& operator <<(std::ostream& out, const Region& region);

private:
	unsigned int m_score;
};

