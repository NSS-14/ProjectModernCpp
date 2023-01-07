#include <iostream>
#include <iostream>
#include <mutex>

#include "LoginHandler.h"
#include "SetTheNumberOfPlayersHandler.h"
#include "Map.h"
#include "Game.h"

int main()
{
	crow::SimpleApp app;

	// Variables:
	Game* game = nullptr;
	std::mutex gameMutex;
	ReadyManager readyManager;
	LoginHandler loginHandler(readyManager);

	// Game initialization:
	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(loginHandler);
	auto& setTheNumberOfPlayersPut = CROW_ROUTE(app, "/host/set-size")
		.methods(crow::HTTPMethod::PUT);
	setTheNumberOfPlayersPut(SetTheNumberOfPlayersHandler(readyManager));

	// Check the ready state and initialize game object:
	CROW_ROUTE(app, "/ready")([&readyManager, &game, &gameMutex, &loginHandler]() {
		if (readyManager.GetOnlinePlayers() == readyManager.GetDesiredNumberOfPlayers()) {
			std::lock_guard<std::mutex> lock(gameMutex);
			if (game == nullptr) {
				game = new Game(readyManager.GetDesiredNumberOfPlayers());

				auto users = loginHandler.GetTheOnlineUsers();
				for (const auto& user : users) {
					game->AddUserAndMakeHimPlayer(user);
				}
			}
			return crow::response(200);
		}
	return crow::response(500);
		});

	// Server functions:
	CROW_ROUTE(app, "/map")([&game]() {
		return game->GetMap().ToString();
		});
	app.port(18080).multithreaded().run();

	return 0;
}