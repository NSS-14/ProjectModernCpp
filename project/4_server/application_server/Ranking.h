#pragma once
#include<queue>
#include "Player.h"
#include <tuple>

class Ranking
{
public:
	using QueueItem = std::tuple<std::shared_ptr<Player>, float, float>;

public:
	struct Compare {
		bool operator ()(QueueItem qi1, QueueItem qi2);
	};

public:
	Ranking() = default;

public:
	void Push(std::shared_ptr<Player> playerIndex, float distanceFromAnswer, float time);
	std::shared_ptr<Player> Pop();

private:
	std::priority_queue<QueueItem, std::vector<QueueItem>, Compare> m_rank;
};
