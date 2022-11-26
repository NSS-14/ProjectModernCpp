#include <iostream>
#include"Ranking.h"

int main()
{
	Ranking r;
	r.Push(1, 71, 1);
	r.Push(0, 1, 5);
	r.Push(2, 1, 3);
	r.Push(3, 100, 5);
	std::cout  <<static_cast <int>(r.Pop());
	std::cout  <<static_cast <int>(r.Pop());
	std::cout  <<static_cast <int>(r.Pop());
	std::cout  <<static_cast <int>(r.Pop());
	return 0;
}