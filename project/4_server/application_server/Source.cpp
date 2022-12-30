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
	map[{0, 1}] = std::make_shared<Player>(Player(4, "darius", "parola"));
	
	auto n = map.Neighbours(map[{0, 0}]);
	for (const auto& e : n) {
		std::cout << static_cast<int>(e.first) << " " << static_cast<int>(e.second) << "\n";
	}
	std::cout << map;
	
	app.port(18080).multithreaded().run();

	return 0;
}