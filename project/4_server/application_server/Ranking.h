#pragma once
#include<queue>

class Ranking
{
public: 
	Ranking() = default;

public:
	using QueueItem = std::tuple<uint8_t, float, float>;

public:
	struct Compare {
		bool operator ()(QueueItem qi1, QueueItem qi2);
	};

public:
	void Push(uint8_t playerIndex, float distanceFromAnswer, float time);
	uint8_t Pop();

private:
	std::priority_queue<QueueItem, std::vector<QueueItem>, Compare> m_rank;


};

