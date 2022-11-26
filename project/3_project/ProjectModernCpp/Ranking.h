#pragma once
#include<queue>

class Ranking
{
public: 
	Ranking() = default;

public:
	using QueueItem = std::pair<uint8_t, float>;

public:
	struct Compare {
		bool operator ()(QueueItem qi1, QueueItem qi2);
	};

public:
	void Push(uint8_t playerIndex, float score);
	uint8_t Pop();

private:
	std::priority_queue<QueueItem, std::vector<QueueItem>, Compare> m_rank;


};

