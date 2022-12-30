#include <iostream>
#include "LoginHandler.h"
#include"Map.h"

int main()
{
	crow::SimpleApp app;

	LoginHandler loginHandler;

	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(loginHandler);

	Map map(3, 3);
	map[{0, 0}] = std::make_shared<Player>(Player(3, "alexia", "parola"));
	std::cout << map;
	
	app.port(18080).multithreaded().run();

	return 0;
}