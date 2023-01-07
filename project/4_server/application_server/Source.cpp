#include <iostream>
#include <iostream>
#include <mutex>

#include "LoginHandler.h"
#include "SetTheNumberOfPlayersHandler.h"
#include "Map.h"

int main()
{
	crow::SimpleApp app;
	ReadyManager readyManager;
	LoginHandler loginHandler(readyManager);

	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(loginHandler);

	auto& setTheNumberOfPlayersPut = CROW_ROUTE(app, "/host/set-size")
		.methods(crow::HTTPMethod::PUT);
	setTheNumberOfPlayersPut(SetTheNumberOfPlayersHandler(readyManager));

	CROW_ROUTE(app, "/ready")([&readyManager]() {
		if (readyManager.GetOnlinePlayers() == readyManager.GetDesiredNumberOfPlayers()) {
			return crow::response(200);
		}
	return crow::response(500);
		});

	Map map(3, 3);

	Player p1(0, "edi", "123", Player::ColorInGame::Blue);
	map[{0, 0}] = std::make_shared<Player>(p1);

	CROW_ROUTE(app, "/map")([&map]() {
		return map.ToString();
		});
	app.port(18080).multithreaded().run();

	return 0;
}