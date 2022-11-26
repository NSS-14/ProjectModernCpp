#include "Ranking.h"

bool Ranking::Compare::operator()(QueueItem qi1, QueueItem qi2)
{
    return qi1.second >= qi2.second;
}
