#pragma once
class Region
{
public:
	static const unsigned int kInitialScore = 100;
	static const unsigned int kStepValue = 100;

public:
	Region();
	Region(unsigned int score);
	unsigned getScore() const;
	void IncrementScore();
	void DecrementScore();

private:
	unsigned int m_score;
};

