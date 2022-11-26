#include "Ranking.h"

bool Ranking::Compare::operator()(QueueItem qi1, QueueItem qi2)
{
    return qi1.second >= qi2.second;
}

void Ranking::Push(uint8_t playerIndex, float score)
{
    m_rank.emplace(playerIndex, score);
}

uint8_t Ranking::Pop()
{
    uint8_t tmp(m_rank.top().first);
    m_rank.pop();
    return tmp;
 }
