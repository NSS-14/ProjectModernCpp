#pragma once
class Region
{
public:
	Region();
	Region(unsigned int score);
	Region getScore() const;
	Region IncrementScore();
	Region DecrementScore();
private:
	unsigned int m_score;
};

