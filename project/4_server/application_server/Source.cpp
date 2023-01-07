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

	Question* currentQuestion = nullptr;
	uint8_t questionReceivers = 0;
	std::vector<std::pair<int, time_t>> beforeQuestionTimes;
	std::vector<std::pair<int, time_t>> answerTimes;
	Ranking currentRanking;
	std::mutex questionMutex;


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
	CROW_ROUTE(app, "/map")([&game, &gameMutex]() {
		std::lock_guard<std::mutex> lock(gameMutex);
		return game->GetMap().ToString();
		});
	CROW_ROUTE(app, "/question/<int>")([&game, &gameMutex, &currentQuestion, &questionReceivers, &questionMutex, &currentRanking, &beforeQuestionTimes](int id) {
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		if (currentQuestion == nullptr) {
			beforeQuestionTimes.clear();
			beforeQuestionTimes.push_back({ id, time(0) });
			std::lock_guard<std::mutex> lockGame(gameMutex);
			currentQuestion = new Question(game->GetQuestion());
		}
		Question q(*currentQuestion);
		if (questionReceivers == game->GetNumberOfPlayers() - 1) {
			delete currentQuestion;
			currentQuestion = nullptr;
		}
		return q.GetQuestion();
		});
	CROW_ROUTE(app, "/answer/<int><string>")([&game, &gameMutex, &questionMutex, &currentRanking, &beforeQuestionTimes, &answerTimes](int id, const std::string& answer) {
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		answerTimes.push_back({ id, time(0) });
		});

	app.port(18080).multithreaded().run();

	return 0;
}