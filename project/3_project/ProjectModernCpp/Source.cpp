#include <iostream>
#include "Player.h"

int main()
{
	Player p1("eduard", "parola");
	std::cout << p1.GetName();

	return 0;
}