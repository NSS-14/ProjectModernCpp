#include "Ranking.h"
#include <iostream>

bool Ranking::Compare::operator()(QueueItem qi1, QueueItem qi2)
{
    if (std::get<1>(qi1) == std::get<1>(qi2))
    {
        return std::get<2>(qi1) > std::get<2>(qi2);
    }
    return std::get<1>(qi1) > std::get<1>(qi2);
}

bool Ranking::Empty() const
{
    return m_rank.empty();
}

void Ranking::Push(std::shared_ptr<Player> player, float distanceFromAnswer, float time)
{
    m_rank.emplace(player, distanceFromAnswer, time);
}

std::shared_ptr<Player> Ranking::Pop()
{
    if (m_rank.empty()) {
        return {};
    }
    std::shared_ptr<Player> tmp(std::get<0>(m_rank.top()));
    m_rank.pop();
    return tmp;
 }
