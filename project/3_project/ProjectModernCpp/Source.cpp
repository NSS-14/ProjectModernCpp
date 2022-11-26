#include <iostream>
#include "Game.h"

int main()
{
	Game g(4);
	Ranking r;

	Player player1("George", "ananas");
	Player player2("Ge21orge", "an1221anas");
	Player player3("Geo4124rge", "an1221anas");
	Player player4("Geo124rge", "anan1241as");

	g.AddPlayer(player1);
	g.AddPlayer(player2);
	g.AddPlayer(player3);
	g.AddPlayer(player4);

	r = g.GiveNumericalQuestionToAll();

	std::cout << static_cast<int>(r.Pop());
	std::cout << static_cast<int>(r.Pop());
	std::cout << static_cast<int>(r.Pop());
	std::cout << static_cast<int>(r.Pop());

	return 0;
}