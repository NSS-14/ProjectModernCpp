#include <iostream>
#include <iostream>
#include <mutex>

#include "LoginHandler.h"
#include "UtilityFunctions.h"
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

	Question currentAskedQuestion;
	std::string replyFormated;
	uint8_t questionReceivers = 0;
	std::unordered_map<std::string, time_t> beforeQuestionTimes;
	std::mutex questionMutex;

	uint8_t answersGived = 0;
	Ranking currentRanking;
	std::mutex answerMutex;

	std::shared_ptr<Player> winner;
	std::mutex winnerMutex;

	// Game initialization:
	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(LoginHandler(readyManager));
	auto& setTheNumberOfPlayersPut = CROW_ROUTE(app, "/host/set_size")
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
		&currentAskedQuestion, &replyFormated, &questionReceivers, &currentRanking, &beforeQuestionTimes, &questionMutex](std::string name)
		{
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		if (questionReceivers == 0) {
			beforeQuestionTimes.clear();
			beforeQuestionTimes[name] = time(0);
			std::lock_guard<std::mutex> lockGame(gameMutex);
			currentAskedQuestion = game->GetNumericalQuestion();
			replyFormated = currentAskedQuestion.GetQuestion() + "\n";
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
	auto& answerPut = CROW_ROUTE(app, "/answer").methods(crow::HTTPMethod::PUT);
	answerPut([&currentAskedQuestion, &beforeQuestionTimes, &questionMutex,
		&game, &gameMutex,
		&answersGived, &currentRanking, &answerMutex](const crow::request& request) {
		auto parsedArguments = ParseUrlArguments(request.body);
		auto end = parsedArguments.end();
		auto nameIterator = parsedArguments.find("name");
		auto answerIterator = parsedArguments.find("answer");
		if (nameIterator == end || answerIterator == end) {
			return crow::response(500);
		}
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		float distanceBetweenAnswers = std::abs(std::stof(currentAskedQuestion.GetAnswer()) - std::stof(answerIterator->second));
		std::lock_guard<std::mutex> lockGame(gameMutex);
		std::lock_guard<std::mutex> lockAnswer(answerMutex);
		currentRanking.Push(game->GetPlayerWithName(nameIterator->second), distanceBetweenAnswers, time(0) - beforeQuestionTimes[nameIterator->second]);
		++answersGived;
		if (answersGived == game->GetNumberOfPlayers()) {
			// to be continued
		}
		return crow::response(200);
		});

	app.port(18080).multithreaded().run();

	return 0;
}