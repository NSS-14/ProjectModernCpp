#include <iostream>
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>

#include "LoginHandler.h"
#include "UtilityFunctions.h"
#include "SetTheNumberOfPlayersHandler.h"
#include "Map.h"
#include "Game.h"

int main()
{
	// App:
	crow::SimpleApp app;
	app.loglevel(crow::LogLevel::Warning);

	// Delay:
	std::chrono::milliseconds delayTime(10);
	std::mutex delayMutex;

	// Variables:
	Game* game = nullptr;
	ReadyManager readyManager;
	std::mutex gameMutex;

	Question currentAskedQuestion;
	std::string replyFormated;
	uint8_t questionReceivers = 0;
	std::unordered_map<std::string, time_t> beforeQuestionTimes;
	std::unordered_map<std::string, std::string> theAnswerGivedByEachPlayer;
	std::mutex questionMutex;

	uint8_t answersGived = 0;
	Ranking currentRanking;
	std::shared_ptr<Player> theGridQuestionWinner;
	std::mutex answerMutex;

	std::shared_ptr<Player> playerInCurrentTurn;
	std::mutex turnMutex;

	size_t remainedRoundsInDuelPhase;
	std::vector<std::shared_ptr<Player>> currentDuelOreder;
	size_t currentPlayerIndex = 0;
	std::vector<Map::Coordinates> currentPlayerNeightbours;
	Map::Coordinates attackedRegionCoordiantes;
	std::shared_ptr<Player> theAttackedPlayer;
	bool drawInGridQuestion = false;
	std::mutex duelMutex;

	// Game initialization:
	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(LoginHandler(readyManager));

	auto& setTheNumberOfPlayersPut = CROW_ROUTE(app, "/host/set_size")
		.methods(crow::HTTPMethod::PUT);
	setTheNumberOfPlayersPut(SetTheNumberOfPlayersHandler(readyManager));


	// Check the ready state and initialize game object:
	CROW_ROUTE(app, "/ready")([&readyManager, &game, &gameMutex, &delayTime, &delayMutex, &remainedRoundsInDuelPhase, &duelMutex]() {
		std::lock_guard<std::mutex> lockDelay(delayMutex);
		std::this_thread::sleep_for(delayTime);
		if (readyManager.GetOnlinePlayers() == readyManager.GetDesiredNumberOfPlayers()) {
			std::lock_guard<std::mutex> lockGame(gameMutex);
			if (game == nullptr) {
				game = new Game(readyManager.GetDesiredNumberOfPlayers());

				auto users = readyManager.GetUsers();
				for (const auto& user : users) {
					game->AddUserAndMakeHimPlayer(user);
				}
				std::lock_guard<std::mutex> lockDuel(duelMutex);
				remainedRoundsInDuelPhase = game->GetNumberOfPlayers() * 2;
			}
			return crow::response(200);
		}
		return crow::response(500);
		});


	// Server functions:
	// Get information from server ( Server -> Client ):
	CROW_ROUTE(app, "/map")([&game, &gameMutex, &delayTime, &delayMutex]() {
		std::lock_guard<std::mutex> lockDelay(delayMutex);
		std::lock_guard<std::mutex> lockGame(gameMutex);
		std::this_thread::sleep_for(delayTime);
		return game->GetMap().ToString();
		});

	CROW_ROUTE(app, "/map_borders")([&game, &gameMutex]() {
		std::lock_guard<std::mutex> lock(gameMutex);
		std::string h = std::to_string(game->GetMap().GetHeight());
		std::string w = std::to_string(game->GetMap().GetWidth());
		return crow::json::wvalue{
				{"height", h},
				{"width", w}
		};
		});

	CROW_ROUTE(app, "/map_is_full")([&game, &gameMutex, &delayTime, &delayMutex, &currentDuelOreder, &duelMutex]() {
		std::lock_guard<std::mutex> lockDelay(delayMutex);
		std::this_thread::sleep_for(delayTime);
		std::lock_guard<std::mutex> lockGame(gameMutex);
		if (game->GetMap().FreeSpacesLeft()) {
			return crow::response(500);
		}
		std::lock_guard<std::mutex> lockDuel(duelMutex);
		if (currentDuelOreder.empty()) {
			currentDuelOreder = game->GetPlayersInRandomOrganizedOrder();
		}
		return crow::response(200);
		});

	CROW_ROUTE(app, "/numerical_question/<string>")([&game, &gameMutex,
		&currentAskedQuestion, &replyFormated, &questionReceivers, &currentRanking, &beforeQuestionTimes, &questionMutex](std::string name)
		{
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		if (questionReceivers == 0) {
			beforeQuestionTimes.clear();
			std::lock_guard<std::mutex> lockGame(gameMutex);
			currentAskedQuestion = game->GetNumericalQuestion();
			replyFormated = currentAskedQuestion.GetQuestion() + "\n";
		}
		beforeQuestionTimes[name] = time(0);
		++questionReceivers;
		std::string reply = replyFormated;
		if (questionReceivers == game->GetNumberOfPlayers()) {
			questionReceivers = 0;
			replyFormated.clear();
		}
		return reply;
		});

	CROW_ROUTE(app, "/grid_question/<string>")([&game, &gameMutex,
		&currentAskedQuestion, &replyFormated, &questionReceivers, &questionMutex](std::string name)
		{
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		if (questionReceivers == 0) {
			std::lock_guard<std::mutex> lockGame(gameMutex);
			currentAskedQuestion = game->GetGridQuestion();
			replyFormated = currentAskedQuestion.GetQuestion() + "\n";
		}
		++questionReceivers;
		std::string reply = replyFormated;
		if (questionReceivers == 2) {
			questionReceivers = 0;
			replyFormated.clear();
		}
		return reply;
		});

	CROW_ROUTE(app, "/is_my_turn/<string>")([&playerInCurrentTurn, &turnMutex, &currentRanking, &answerMutex, &delayTime, &delayMutex](std::string name) {
		std::lock_guard<std::mutex> lockDelay(delayMutex);
		std::this_thread::sleep_for(delayTime);
		std::lock_guard<std::mutex> lockTurn(turnMutex);
		if (playerInCurrentTurn.get() == nullptr) {
			return crow::response(500);
		}
		if (playerInCurrentTurn->GetName() == name) {
			return crow::response(200);
		}
		return crow::response(500);
		});

	CROW_ROUTE(app, "/is_my_turn_to_choose_my_oponent/<string>")([&currentDuelOreder, &currentPlayerIndex, &duelMutex, &delayTime, &delayMutex](std::string name) {
		std::lock_guard<std::mutex> lockDelay(delayMutex);
		std::this_thread::sleep_for(delayTime);
		std::lock_guard<std::mutex> lockDuel(duelMutex);
		if (currentDuelOreder[currentPlayerIndex]->GetName() == name) {
			return crow::response(200);
		}
		return crow::response(500);
		});

	CROW_ROUTE(app, "/show_my_oponents_coordinates")([&currentDuelOreder, &currentPlayerIndex, &currentPlayerNeightbours, &duelMutex, &game, &gameMutex]() {
		std::lock_guard<std::mutex> lockDuel(duelMutex);
		std::lock_guard<std::mutex> lockGame(gameMutex);
		std::vector<crow::json::wvalue> neightboursJson;

		currentPlayerNeightbours = game->GetMap().Neighbours(currentDuelOreder[currentPlayerIndex]);
		for (const Map::Coordinates& coordinates : currentPlayerNeightbours) {
			neightboursJson.push_back(crow::json::wvalue{
				{"x", coordinates.first},
				{"y", coordinates.second}
				});
		}
		return crow::json::wvalue{ neightboursJson };
		});

	CROW_ROUTE(app, "/is_my_turn_to_fight_back_my_oponent/<string>")([&theAttackedPlayer, &duelMutex, &delayTime, &delayMutex](std::string name) {
		std::lock_guard<std::mutex> lockDelay(delayMutex);
		std::this_thread::sleep_for(delayTime);
		std::lock_guard<std::mutex> lockDuel(duelMutex);
		if (theAttackedPlayer.get() == nullptr) {
			return crow::response(500);
		}
		if (theAttackedPlayer->GetName() == name) {
			return crow::response(200);
		}
		return crow::response(500);
		});

	CROW_ROUTE(app, "/phase_done")([&playerInCurrentTurn, &turnMutex, &delayTime, &delayMutex]() {
		std::lock_guard<std::mutex> lockDelay(delayMutex);
		std::this_thread::sleep_for(delayTime);
		std::lock_guard<std::mutex> lockAnswer(turnMutex);
		if (playerInCurrentTurn.get() == nullptr) {
			return crow::response(200);
		}
		return crow::response(500);
		});

	CROW_ROUTE(app, "/how_many_regions")([&game, &gameMutex, &currentRanking, &answerMutex]() {
		std::lock_guard<std::mutex> lockGame(gameMutex);
		std::lock_guard<std::mutex> lockAnswer(answerMutex);
		size_t freeSpaces = game->GetMap().FreeSpacesLeft();
		if (freeSpaces < currentRanking.Size()) {
			return std::to_string(freeSpaces);
		}
		return std::to_string(currentRanking.Size());
		});

	CROW_ROUTE(app, "/duels_are_done")([&remainedRoundsInDuelPhase, &duelMutex]() {
		std::lock_guard<std::mutex> lockDuel(duelMutex);
		if (!remainedRoundsInDuelPhase) {
			return crow::response(200);
		}
		return crow::response(500);
		});


	// Set information on server ( Client -> Server ):
	auto& numericalAnswerPut = CROW_ROUTE(app, "/numerical_answer").methods(crow::HTTPMethod::PUT);
	numericalAnswerPut([&currentAskedQuestion, &beforeQuestionTimes, &questionMutex,
		&game, &gameMutex,
		&answersGived, &currentRanking, &answerMutex,
		&playerInCurrentTurn, &turnMutex](const crow::request& request) {
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
			std::lock_guard<std::mutex> lockTurn(turnMutex);
			answersGived = 0;
			playerInCurrentTurn = currentRanking.Pop();
		}
		return crow::response(200);
		});

	auto& gridAnswerPut = CROW_ROUTE(app, "/grid_answer").methods(crow::HTTPMethod::PUT);
	gridAnswerPut([&currentAskedQuestion, &theAnswerGivedByEachPlayer, &questionMutex,
		&game, &gameMutex,
		&answersGived, &theGridQuestionWinner, &answerMutex,
		&theAttackedPlayer, &currentDuelOreder, &currentPlayerIndex, &drawInGridQuestion, &duelMutex](const crow::request& request) {
		auto parsedArguments = ParseUrlArguments(request.body);
		auto end = parsedArguments.end();
		auto nameIterator = parsedArguments.find("name");
		auto answerIterator = parsedArguments.find("answer");
		if (nameIterator == end || answerIterator == end) {
			return crow::response(500);
		}
		std::lock_guard<std::mutex> lockQuestion(questionMutex);
		theAnswerGivedByEachPlayer[nameIterator->second] = answerIterator->second;
		std::lock_guard<std::mutex> lockAnswer(answerMutex);
		++answersGived;
		if (answersGived == 2) {
			bool attackedWon = false;
			bool attackerWon = false;
			if (theAnswerGivedByEachPlayer[theAttackedPlayer->GetName()] == currentAskedQuestion.GetAnswer()) {
				attackedWon = true;
			}
			if (theAnswerGivedByEachPlayer[currentDuelOreder[currentPlayerIndex]->GetName()] == currentAskedQuestion.GetAnswer()) {
				attackerWon = true;
			}
			if (attackedWon && attackerWon) {
				drawInGridQuestion = true;
			}
			else {
				drawInGridQuestion = false;
				if (attackedWon) {
					theGridQuestionWinner = theAttackedPlayer;
				}
				else if (attackerWon) {
					theGridQuestionWinner = currentDuelOreder[currentPlayerIndex];
				}
				else {
					theGridQuestionWinner.reset();
				}
			}
		}
		return crow::response(200);
		});

	CROW_ROUTE(app, "/set_base/<string>")([&playerInCurrentTurn, &turnMutex, &currentRanking, &answerMutex, &game, &gameMutex](std::string coordinates) {
		uint8_t x = std::stoi(coordinates.substr(0, 1));
		uint8_t y = std::stoi(coordinates.substr(1, 1));
		std::lock_guard<std::mutex> lockGame(gameMutex);
		if (game->GetMap()[{x, y}].get() != nullptr) {
			return crow::response(500);
		}
		std::lock_guard<std::mutex> lockTurn(turnMutex);
		game->GetMap()[{x, y}] = playerInCurrentTurn;
		playerInCurrentTurn->AddNewRegionAt({ x, y });
		std::lock_guard<std::mutex> lockAnswer(answerMutex);
		playerInCurrentTurn = currentRanking.Pop();
		return crow::response(200);
		});

	CROW_ROUTE(app, "/set_region/<string>")([&playerInCurrentTurn, &turnMutex, &game, &gameMutex](std::string coordinates) {
		uint8_t x = std::stoi(coordinates.substr(0, 1));
		uint8_t y = std::stoi(coordinates.substr(1, 1));
		std::lock_guard<std::mutex> lockGame(gameMutex);
		if (game->GetMap()[{x, y}].get() != nullptr) {
			return crow::response(500);
		}
		std::lock_guard<std::mutex> lockTurn(turnMutex);
		game->GetMap()[{x, y}] = playerInCurrentTurn;
		playerInCurrentTurn->AddNewRegionAt({ x, y });
		return crow::response(200);
		});

	CROW_ROUTE(app, "/go_next_player")([&playerInCurrentTurn, &turnMutex, &currentRanking, &answerMutex]() {
		std::lock_guard<std::mutex> lockTurn(turnMutex);
		std::lock_guard<std::mutex> lockAnswer(answerMutex);
		playerInCurrentTurn = currentRanking.Pop();
		return crow::response(200);
		});

	CROW_ROUTE(app, "/choose_my_oponent_coordinates/<string>")([&currentDuelOreder, &currentPlayerIndex, &currentPlayerNeightbours, &theAttackedPlayer, &attackedRegionCoordiantes, &duelMutex, &game, &gameMutex](std::string coordinates) {
		uint8_t x = std::stoi(coordinates.substr(0, 1));
		uint8_t y = std::stoi(coordinates.substr(1, 1));
		attackedRegionCoordiantes = { x, y };
		bool testIfTheCoordinatesAreInsideNeightboursVector = false;
		std::lock_guard<std::mutex> lockDuel(duelMutex);
		for (const Map::Coordinates& coordinates : currentPlayerNeightbours) {
			if (coordinates == attackedRegionCoordiantes) {
				testIfTheCoordinatesAreInsideNeightboursVector = true;
				break;
			}
		}
		if (!testIfTheCoordinatesAreInsideNeightboursVector) {
			return crow::response(500);
		}
		std::lock_guard<std::mutex> lockGame(gameMutex);
		theAttackedPlayer = game->GetMap()[attackedRegionCoordiantes];
		return crow::response(200);
		});

	app.port(18080).multithreaded().run();

	return 0;
}