#pragma once
class Region
{
public:
	Region();
	Region(unsigned int score);
	unsigned getScore() const;
	void IncrementScore();
	void DecrementScore();
private:
	unsigned int m_score;
};

