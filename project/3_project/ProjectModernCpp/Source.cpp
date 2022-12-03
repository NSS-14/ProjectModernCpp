#include <iostream>
#include"Game.h"

int main()
{
	Game g1(4);
	Player p1("Ana", "1234");
	Player p2("Ion", "wuheerer");
	Player p3("Oana", "cjcjc");
	Player p4("Andrei", "09404");

	g1.AddPlayer(p1);
	g1.AddPlayer(p2);
	g1.AddPlayer(p3);
	g1.AddPlayer(p4);

	g1.Start();

	return 0;
}