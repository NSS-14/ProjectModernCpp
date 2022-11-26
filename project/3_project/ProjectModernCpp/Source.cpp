#include <iostream>
#include "Player.h"

int main()
{
	Player p1("eduard", "parola");
	std::cout << p1.GetName();
	std::cout << p1.GetPassword();

	Region regiune(100, {0, 0});

	p1.InsertRegion(regiune);
	std::cout << p1.ExtractRegion({0, 0});
	return 0;
}