#include <iostream>
#include "Game.h"

int main()
{
	Game g(4);

	Player player1("George", "ananas");
	Player player2("Ge21orge", "an1221anas");
	Player player3("Geo4124rge", "an1221anas");
	Player player4("Geo124rge", "anan1241as");

	g.AddPlayer(player1);
	g.AddPlayer(player2);
	g.AddPlayer(player3);
	g.AddPlayer(player4);

	g.Start();

	return 0;
}