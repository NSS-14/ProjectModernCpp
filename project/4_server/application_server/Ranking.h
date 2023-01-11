#pragma once
#include <queue>
#include "Player.h"
#include <tuple>
#include <memory>

class Ranking
{
public:
	// The first element of tuple represents the Player that gived an answer
	// The second elemnt represents the absolute value of diffrence between right answer and the given answer
	// The third element from this tuple is the amount of time that passed since the question was asked and the player answerd it
	using QueueItem = std::tuple<std::shared_ptr<Player>, float, float>;

public:
	struct Compare {
		bool operator ()(QueueItem qi1, QueueItem qi2);
	};

public:
	Ranking() = default;

public:
	bool Empty() const;

public:
	void Push(std::shared_ptr<Player> player, float distanceFromAnswer, float time);
	std::shared_ptr<Player> Pop();

private:
	std::priority_queue<QueueItem, std::vector<QueueItem>, Compare> m_rank;
};
