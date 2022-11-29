#include "Ranking.h"

bool Ranking::Compare::operator()(QueueItem qi1, QueueItem qi2)
{
    if (std::get<1>(qi1) == std::get<1>(qi2))
    {
        return std::get<2>(qi1) > std::get<2>(qi2);
    }
    return std::get<1>(qi1) > std::get<1>(qi2);
}

void Ranking::Push(uint8_t playerIndex, float distanceFromAnswer, float time)
{
    m_rank.emplace(playerIndex, distanceFromAnswer, time);
}

uint8_t Ranking::Pop()
{
    uint8_t tmp(std::get<0>(m_rank.top()));
    m_rank.pop();
    return tmp;
 }
