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
	ReadyManager readyManager;
	std::mutex gameMutex;

	std::string replyFormated;
	uint8_t questionReceivers = 0;
	std::unordered_map<std::string, time_t> beforeQuestionTimes;
	std::mutex questionMutex;

	std::unordered_map<std::string, time_t> answerTimes;
	Ranking currentRanking;
	std::mutex answerMutex;

	// Game initialization:
	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(LoginHandler(readyManager));
	auto& setTheNumberOfPlayersPut = CROW_ROUTE(app, "/host/set-size")
		.methods(crow::HTTPMethod::PUT);
	setTheNumberOfPlayersPut(SetTheNumberOfPlayersHandler(readyManager));

	// Check the ready state and initialize game object:
	CROW_ROUTE(app, "/ready")([&readyManager, &game, &gameMutex]() {
		if (readyManager.GetOnlinePlayers() == readyManager.GetDesiredNumberOfPlayers()) {
			std::lock_guard<std::mutex> lock(gameMutex);
			if (game == nullptr) {
				game = new Game(readyManager.GetDesiredNumberOfPlayers());

				auto users = readyManager.GetUsers();
				for (const auto& user : users) {
					game->AddUserAndMakeHimPlayer(user);
				}
			}
			return crow::response(200);
		}
		return crow::response(500);
		});

	// Server functions:
	// Get information from server ( Server -> Client ):
	CROW_ROUTE(app, "/map")([&game, &gameMutex]() {
		std::lock_guard<std::mutex> lock(gameMutex);
		return game->GetMap().ToString();
		});
	CROW_ROUTE(app, "/numerical_question/<string>")([&game, &gameMutex,
		&replyFormated, &questionReceivers, &currentRanking, &beforeQuestionTimes, &questionMutex](std::string name)
		{
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		if (questionReceivers == 0) {
			beforeQuestionTimes.clear();
			beforeQuestionTimes[name] = time(0);
			std::lock_guard<std::mutex> lockGame(gameMutex);
			const Question& currentQuestion = game->GetNumericalQuestion();
			replyFormated = currentQuestion.GetQuestion() + "\n";
		}
		++questionReceivers;
		std::string reply = replyFormated;
		if (questionReceivers == game->GetNumberOfPlayers()) {
			questionReceivers = 0;
			replyFormated.clear();
		}
		return reply;
		});

	// Set information on server ( Client -> Server ):
	/*CROW_ROUTE(app, "/answer/<int><string>")([&game, &gameMutex, &questionMutex, &currentRanking, &beforeQuestionTimes, &answerTimes](int id, const std::string& answer) {
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		answerTimes.push_back({ id, time(0) });
		});*/

	app.port(18080).multithreaded().run();

	return 0;
}