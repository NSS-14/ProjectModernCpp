#include <iostream>
#include"Game.h"

int main()
{
	Game g1(2);
	Player p1("Ana", "1234");
	Player p2("Ion", "wuheerer");
	
	

	g1.AddPlayer(p1);
	g1.AddPlayer(p2);
	


	g1.Start();

	return 0;
}