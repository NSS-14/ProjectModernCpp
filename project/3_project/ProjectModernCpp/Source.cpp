#include <iostream>
#include"Ranking.h"

int main()
{
	Ranking r;
	r.Push(1, 71);
	r.Push(0, 1);
	r.Push(2, 5);
	r.Push(3, 100);
	std::cout  <<static_cast <int>(r.Pop());
	return 0;
}