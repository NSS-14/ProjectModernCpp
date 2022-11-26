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

private:
	std::priority_queue<QueueItem, std::vector<QueueItem>, Compare> m_rank;


};

