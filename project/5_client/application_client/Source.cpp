#include <iostream>
#include <string>

#include <cpr/cpr.h>
#include <crow.h>
#include <regex>
#include <cstdint>
#include <chrono>
#include <thread>

enum class LoginState {
	Host,
	Wait,
	Error
};
enum class WhatAmI {
	Attacker,
	Attacked
};
enum class DuelResult {
	Draw,
	Win,
	Lose
};
LoginState LoginMenu(std::string& name, std::string& password);

std::string GetMap();
std::pair<uint8_t, uint8_t> GetMapBorders();
std::string GetNumericalQuestion(const std::string& name);
std::string GetGridQuestion();
WhatAmI GetWhatAmI(const std::string& name);
void GetAttackableRegionsCoordinates();

void SetGameSize();
void SetNumericalAnswer(const std::string& name);
void SetGridAnswer(const std::string& name);
void SetBase(const std::pair<uint8_t, uint8_t>& borders);
void SetRegions(const std::pair<uint8_t, uint8_t>& borders);
void SetTheRegionYouWantToAttack(const std::pair<uint8_t, uint8_t>& borders);
void SetAdvantageUseRegion(const std::string& name, const std::pair<uint8_t, uint8_t>& borders);

bool UseAdvantageForCurrentQuestion(const std::string& name, bool currentQuestionIsNumerical);

void WaitForAllPlayersToLogin();
void WaitForMyTurnToPlaceMyBase(const std::string& name);
void WaitForTheRestOfThePlayersToSetTheirBase();
void WaitForMyTurnToPlaceMyRegions(const std::string& name);
void WaitForTheRestOfThePlayersToSetTheirRegions();
void WaitForBouthDuelPlayersToAnswer();
void WaitForBouthDuelPlayersToAnswerNumericalQuestion();

bool TestIfFillMapPhaseIsDone();
bool TestIfDuelsAreDone();
bool TestIfIAmAttacker(const std::string& name);
bool TestIfIAmAttacked(const std::string& name);
bool TestIfItWasDrawOnGridQuestion();

bool LoginPhase(std::string& name, std::string& password);
void ChosingBasePhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);
void FillMapPhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);
void DuelPhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);
void ResultsPhase();

void AttackerBehaviour(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);
void AttackedBehaviour(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);
void ShowDuelResult(const std::string& name);

int main()
{
	// Variables:
	std::string name;
	std::string password;
	std::pair<uint8_t, uint8_t> mapBorders;

	// Login phase:
	if (!LoginPhase(name, password)) {
		return 1;
	}

	// Chosing base phase:
	mapBorders = GetMapBorders();
	ChosingBasePhase(name, mapBorders);

	// Filling the map with regions phase:
	FillMapPhase(name, mapBorders);

	// Duel phase:
	DuelPhase(name, mapBorders);

	// Results phase:
	ResultsPhase();

	return 0;
}

LoginState LoginMenu(std::string& name, std::string& password) {
	while (true) {
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::system("CLS");
		std::cout << "Login menu:\n";
		std::cout << "Name: "; std::cin >> name;
		std::cout << "Password: "; std::cin >> password;

		auto response = cpr::Put(
			cpr::Url{ "http://localhost:18080/login" },
			cpr::Payload{
				{ "name", name },
				{ "password", password }
			}
		);
		if (response.status_code == 201) {
			std::cout << "Your account was successfully logged in and you're the game host!\n";
			std::system("PAUSE");
			return LoginState::Host;
		}
		if (response.status_code == 202) {
			std::cout << "Your account was successfully logged in! You have to wait for the rest of the players.\n";
			std::system("PAUSE");
			return LoginState::Wait;
		}
		if (response.status_code == 400) {
			std::cout << "Your account was not logged in! Your client app is broken. The login formulation is spelt wrong by the app.\n";
			std::system("PAUSE");
			return LoginState::Error;
		}
		if (response.status_code != 401 && response.status_code != 403 && response.status_code != 0) {
			std::cout << "There was an error on the server. We are sorry!\nError code: " << response.status_code << '\n';
			std::system("PAUSE");
			return LoginState::Error;
		}
		if (response.status_code == 0) std::cout << "The server is offline. Try again later!";
		if (response.status_code == 401) std::cout << "Your account was not logged on becouse the inserted password is incorect.";
		if (response.status_code == 403) std::cout << "Error! You are already logged in. Change password if it is not you.";
		std::cout << "\nPress 1 and enter to retry the login operation.\n";
		int choice; std::cin >> choice;
		if (choice != 1) {
			return LoginState::Error;
		}
	}
}

std::string GetMap()
{
	auto response = cpr::Get(cpr::Url{ "http://localhost:18080/map" });
	return response.text;
}
std::pair<uint8_t, uint8_t> GetMapBorders()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/map_borders" });
	auto responseRows = crow::json::load(response.text);
	int h = responseRows["height"].i();
	int w = responseRows["width"].i();
	return { h, w };
}
std::string GetNumericalQuestion(const std::string& name)
{
	auto response = cpr::Get(cpr::Url{ "http://localhost:18080/numerical_question/" + name });
	return response.text;
}
std::string GetGridQuestion()
{
	auto response = cpr::Get(cpr::Url{ "http://localhost:18080/grid_question" });
	return response.text;
}
WhatAmI GetWhatAmI(const std::string& name)
{
	std::system("CLS");
	std::cout << "Wait for your turn to attack or prepare to be attacked!\n";
	std::string lastMap = GetMap();
	std::cout << lastMap;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		if (TestIfIAmAttacked(name)) {
			return WhatAmI::Attacked;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		if (TestIfIAmAttacker(name)) {
			return WhatAmI::Attacker;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		if (lastMap != GetMap()) {
			std::string lastMap = GetMap();
			std::system("CLS");
			std::cout << "Wait for your turn to attack or prepare to be attacked!\n";
			std::cout << lastMap;
		}
	}
}
void GetAttackableRegionsCoordinates()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/show_my_oponents_coordinates" });

	auto regions = crow::json::load(response.text);

	std::cout << "Here are the attackable regions:\n";
	for (const auto& region : regions) {
		std::cout << '(' << region["x"] << ", " << region["y"] << ") ";
	}
	std::cout << std::endl;
}

void SetGameSize()
{
	std::string gameSize;
	std::regex digitIntervalRegex("[2-4]");

	while (true) {
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::system("CLS");
		std::cout << "Insert the game size: ";
		std::cin >> gameSize;
		if (!std::regex_match(gameSize, digitIntervalRegex)) {
			std::cout << "You have to insert a number between 2 and 4!\n";
			std::system("PAUSE");
			continue;
		}
		break;
	}
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/host/set_size" },
		cpr::Payload{
			{ "size", gameSize }
		}
	);
	if (response.status_code == 200) {
		std::cout << "The game size is " << gameSize << '.' << std::endl;
	}
}
void SetNumericalAnswer(const std::string& name)
{
	std::string answer;
	std::regex number("[0-9]|([1-9][0-9]+)");

	while (true) {
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::cout << "Your answer: ";
		std::cin >> answer;
		if (!std::regex_match(answer, number)) {
			std::cout << "Your input is not a number. Try again!\n";
			continue;
		}
		break;
	}
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/numerical_answer" },
		cpr::Payload{
			{ "name", name },
			{ "answer", answer }
		}
	);
	if (response.status_code == 200) {
		std::cout << "Your answer was succesfuly recevied by the game!\n";
	}
}
void SetGridAnswer(const std::string& name)
{
	std::string answer;
	std::cin.clear();
	std::cin.seekg(std::ios::end);
	std::cout << "Your answer: ";
	std::getline(std::cin, answer);
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/grid_answer" },
		cpr::Payload{
			{ "name", name },
			{ "answer", answer }
		}
	);
	if (response.status_code == 200) {
		std::cout << "Your answer was succesfuly recevied by the game!\n";
	}
}
void SetBase(const std::pair<uint8_t, uint8_t>& borders)
{
	std::pair<std::string, std::string> answer;
	std::regex number("[0-9]|([1-9][0-9]+)");

	while (true) {
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::cout << "Insert your base coordinates: ";
		std::cin >> answer.first >> answer.second;
		if (!std::regex_match(answer.first, number) || !std::regex_match(answer.second, number)) {
			std::cout << "Your input is not a number. Try again!\n";
			continue;
		}
		if (std::stoi(answer.first) >= borders.first || std::stoi(answer.second) >= borders.second) {
			std::cout << "Your input is out of bounds. Try again!\n";
			continue;
		}
		auto response = cpr::Get(cpr::Url{ "http://localhost:18080/set_base/" + answer.first + answer.second });
		if (response.status_code != 200) {
			std::cout << "The place is already ocupied. Try another place!\n";
			continue;
		}
		break;
	}
}
void SetRegions(const std::pair<uint8_t, uint8_t>& borders)
{
	std::pair<std::string, std::string> answer;
	std::regex number("[0-9]|([1-9][0-9]+)");

	auto responseNumberOfRegionsIcanPlace = cpr::Get(cpr::Url{ "http://localhost:18080/how_many_regions" });
	int numberOfRegionsIcanPlace = std::stoi(responseNumberOfRegionsIcanPlace.text);

	for (int i = 0; i < numberOfRegionsIcanPlace; ++i) {
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::system("CLS");
		std::cout << "Now is your turn! You shoud place " << numberOfRegionsIcanPlace << " regions!\n";
		std::cout << GetMap();
		std::cout << "Insert your region (" << i + 1 << ") coordinates: ";
		std::cin >> answer.first >> answer.second;
		if (!std::regex_match(answer.first, number) || !std::regex_match(answer.second, number)) {
			std::cout << "Your input is not a number. Try again!\n";
			--i;
			std::system("PAUSE");
			continue;
		}
		if (std::stoi(answer.first) >= borders.first || std::stoi(answer.second) >= borders.second) {
			std::cout << "Your input is out of bounds. Try again!\n";
			--i;
			std::system("PAUSE");
			continue;
		}
		auto response = cpr::Get(cpr::Url{ "http://localhost:18080/set_region/" + answer.first + answer.second });
		if (response.status_code != 200) {
			std::cout << "The place is already ocupied. Try another place!\n";
			--i;
			std::system("PAUSE");
			continue;
		}
	}
	auto responeNext = cpr::Get(cpr::Url{ "http://localhost:18080/go_next_player" });
}
void SetTheRegionYouWantToAttack(const std::pair<uint8_t, uint8_t>& borders)
{
	std::pair<std::string, std::string> answer;
	std::regex number("[0-9]|([1-9][0-9]+)");

	while (true) {
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::cout << "Insert the coordinates of the region you want to attack: ";
		std::cin >> answer.first >> answer.second;
		if (!std::regex_match(answer.first, number) || !std::regex_match(answer.second, number)) {
			std::cout << "Your input is not a number. Try again!\n";
			continue;
		}
		if (std::stoi(answer.first) >= borders.first || std::stoi(answer.second) >= borders.second) {
			std::cout << "Your input is out of bounds. Try again!\n";
			continue;
		}
		auto response = cpr::Get(cpr::Url{ "http://localhost:18080/choose_my_oponent_coordinates/" + answer.first + answer.second });
		if (response.status_code != 200) {
			std::cout << "The region you choosed is not your neightbour or it is you and you can't attack yourself. Try another place!\n";
			continue;
		}
		break;
	}
}
void SetAdvantageUseRegion(const std::string& name, const std::pair<uint8_t, uint8_t>& borders)
{
	std::pair<std::string, std::string> answer;
	std::regex number("[0-9]|([1-9][0-9]+)");

	while (true) {
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::cout << "Insert the region you want to decrement score: ";
		std::cin >> answer.first >> answer.second;
		if (!std::regex_match(answer.first, number) || !std::regex_match(answer.second, number)) {
			std::cout << "Your input is not a number. Try again!\n";
			continue;
		}
		if (std::stoi(answer.first) >= borders.first || std::stoi(answer.second) >= borders.second) {
			std::cout << "Your input is out of bounds. Try again!\n";
			continue;
		}
		auto response = cpr::Get(cpr::Url{ "http://localhost:18080/decrement_region_becouse_useage_of_advantage/" + answer.first + answer.second + name});
		if (response.status_code != 200) {
			std::cout << "The region is not yours or it has a score below 200! Try again!\n";
			continue;
		}
		break;
	}
}

bool UseAdvantageForCurrentQuestion(const std::string& name, bool currentQuestionIsNumerical)
{
	std::cout << "Do you want to use any advantage for this question?\n";
	std::cout << "Type 1 if yes!\n";
	int inputFromClient;
	std::cin.clear();
	std::cin.seekg(std::ios::end);
	std::cin >> inputFromClient;
	if (inputFromClient != 1) {
		return false;
	}

	cpr::Response responseJsonWithAdvantages = cpr::Get(cpr::Url{ "http://localhost:18080/get_my_advantages/" + name });

	auto advantages = crow::json::load(responseJsonWithAdvantages.text);
	bool haveAtLeasOne = false;
	std::cout << "Here are your advantages:\n";
	for (const auto& advantage : advantages) {
		std::cout << advantage["name"] << " ";
		if (advantage["available"].b()) {
			std::cout << "available";
			haveAtLeasOne = true;
		}
		else {
			std::cout << "unavailable";
		}
		std::cout << "; ";
	}
	std::cout << std::endl;
	if (!haveAtLeasOne) {
		std::cout << "You can't use any advantages becouse you don't have any left!\n";
		return false;
	}
	cpr::Response responseCanIuseMyAdvantages = cpr::Get(cpr::Url{ "http://localhost:18080/can_use_my_advantages/" + name });
	if (responseCanIuseMyAdvantages.status_code != 200) {
		std::cout << "You can't use any advantages becouse you don't have any region with score greather than 200!\n";
		return false;
	}

	while (true) {
		std::cout << "Type 1 to use the FiftyFifty advantage. Type 2 to use the MakeItGrid advantage. Type 3 to use the Suggestion advantage.\n";
		std::cin.clear();
		std::cin.seekg(std::ios::end);
		std::cin >> inputFromClient;
		if (inputFromClient < 1 || inputFromClient > 3) {
			std::cout << "Retry inserting the option in interval [1,3].\n";
			continue;
		}
		if (currentQuestionIsNumerical && inputFromClient == 1) {
			std::cout << "Retry inserting the option in interval [2,3] Becouse you can't use advantage one on numerical questions.\n";
			continue;
		}
		if (!currentQuestionIsNumerical && (inputFromClient == 2 || inputFromClient == 3)) {
			std::cout << "Retry inserting the option in interval [1] Becouse you can't use advantages 2 or 3 on grid questions.\n";
			continue;
		}
		cpr::Response advantageResponse;
		switch (inputFromClient)
		{
		case 1: {
			advantageResponse = cpr::Get(cpr::Url{ "http://localhost:18080/use_fifty_fifty_advantage/" + name });
			if (!advantageResponse.text.size()) {
				std::cout << "You used an unavallable advantage.\n";
				return false;
			}
			else {
				std::cout << advantageResponse.text << std::endl;
				return true;
			}
			break;
		}
		case 2: {
			advantageResponse = cpr::Get(cpr::Url{ "http://localhost:18080/use_choose_answer_advantage/" + name });
			if (!advantageResponse.text.size()) {
				std::cout << "You used an unavallable advantage.\n";
				return false;
			}
			else {
				std::cout << advantageResponse.text << std::endl;
				return true;
			}
			break;
		}
		case 3: {
			advantageResponse = cpr::Get(cpr::Url{ "http://localhost:18080/use_suggestion_advantage/" + name });
			if (!advantageResponse.text.size()) {
				std::cout << "You used an unavallable advantage.\n";
				return false;
			}
			else {
				std::cout << advantageResponse.text << std::endl;
				return true;
			}
			break;
		}
		}
		break;
	}
}

void WaitForAllPlayersToLogin()
{
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/ready" });
		if (response.status_code == 200) {
			break;
		}
	}
}
void WaitForMyTurnToPlaceMyBase(const std::string& name)
{
	std::system("CLS");
	std::cout << "Wait for your turn to place your base.\n";
	std::string lastMap = GetMap();
	std::cout << lastMap;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/is_my_turn/" + name });
		if (response.status_code == 200) {
			break;
		}
		if (lastMap != GetMap()) {
			std::system("CLS");
			std::cout << "Wait for your turn to place your base.\n";
			lastMap = GetMap();
			std::cout << lastMap;
		}
	}
}
void WaitForTheRestOfThePlayersToSetTheirBase()
{
	std::system("CLS");
	std::cout << "You have placed your base succesfuly! Now wait for the rest of the players!\n";
	std::string lastMap = GetMap();
	std::cout << lastMap;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/phase_done" });
		if (response.status_code == 200) {
			break;
		}
		if (lastMap != GetMap()) {
			std::system("CLS");
			std::cout << "You have placed your base succesfuly! Now wait for the rest of the players!\n";
			lastMap = GetMap();
			std::cout << lastMap;
		}
	}
}
void WaitForMyTurnToPlaceMyRegions(const std::string& name)
{
	std::system("CLS");
	std::cout << "Wait for your turn to place your regions.\n";
	std::string lastMap = GetMap();
	std::cout << lastMap;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/is_my_turn/" + name });
		if (response.status_code == 200) {
			break;
		}
		if (lastMap != GetMap()) {
			std::system("CLS");
			std::cout << "Wait for your turn to place your regions.\n";
			lastMap = GetMap();
			std::cout << lastMap;
		}
	}
}
void WaitForTheRestOfThePlayersToSetTheirRegions()
{
	std::system("CLS");
	std::cout << "You are done! Now wait for all players to set their regions.\n";
	std::string lastMap = GetMap();
	std::cout << lastMap;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/phase_done" });
		if (response.status_code == 200) {
			break;
		}
		if (lastMap != GetMap()) {
			std::system("CLS");
			std::cout << "You are done! Now wait for all players to set their regions.\n";
			lastMap = GetMap();
			std::cout << lastMap;
		}
	}
}
void WaitForBouthDuelPlayersToAnswer()
{
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/test_duel_question_is_answered_by_bouth_of_us" });
		if (response.status_code == 200) {
			break;
		}
	}
}
void WaitForBouthDuelPlayersToAnswerNumericalQuestion()
{
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/test_duel_question_is_answered_by_bouth_of_us_numerical" });
		if (response.status_code == 200) {
			break;
		}
	}
}

bool TestIfFillMapPhaseIsDone()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/map_is_full" });
	if (response.status_code == 200) {
		return true;
	}
	return false;
}
bool TestIfDuelsAreDone()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/duels_are_done" });
	if (response.status_code == 200) {
		return true;
	}
	return false;
}
bool TestIfIAmAttacker(const std::string& name)
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/is_my_turn_to_choose_my_oponent/" + name });
	if (response.status_code == 200) {
		return true;
	}
	return false;
}
bool TestIfIAmAttacked(const std::string& name)
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/is_my_turn_to_fight_back_my_oponent/" + name });
	if (response.status_code == 200) {
		return true;
	}
	return false;
}
bool TestIfItWasDrawOnGridQuestion()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/test_for_draw_in_duel_grid_question" });
	if (response.status_code == 200) {
		return true;
	}
	return false;
}

bool LoginPhase(std::string& name, std::string& password)
{
	LoginState loginState = LoginMenu(name, password);
	if (loginState == LoginState::Error) {
		return false;
	}
	if (loginState == LoginState::Host) {
		SetGameSize();
	}
	WaitForAllPlayersToLogin();
	std::system("CLS");
	std::cout << "The game is ready! All players are loged in!\n";
	std::cout << GetMap();
	std::cout << "Press any key to start the first phase of the game!\n";
	std::system("PAUSE");
	return true;
}
void ChosingBasePhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders)
{
	std::system("CLS");
	std::cout << "This is the chosing base phase! We will ask you a question, answer it as fast as you can!\n";
	std::system("PAUSE");
	std::system("CLS");
	std::cout << GetNumericalQuestion(name);
	SetNumericalAnswer(name);
	std::cout << "Press any key to place your base!\n";
	std::system("PAUSE");
	WaitForMyTurnToPlaceMyBase(name);

	std::system("CLS");
	std::cout << "Now is your turn!\n";
	std::cout << GetMap();
	SetBase(mapBorders);
	WaitForTheRestOfThePlayersToSetTheirBase();

	std::system("CLS");
	std::cout << "The chosing base phase is done! All players setted their bases!\n";
	std::cout << GetMap();
	std::cout << "Press any key to go to the next phase!\n";
	std::system("PAUSE");
}
void FillMapPhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders)
{
	std::system("CLS");
	std::cout << "This is the filling the map phase! We will ask qouestions until the map gets full of regions! Get ready!\n";
	std::system("PAUSE");
	while (true) {
		std::system("CLS");
		if (TestIfFillMapPhaseIsDone()) {
			break;
		}
		std::cout << GetNumericalQuestion(name);
		SetNumericalAnswer(name);
		std::cout << "Press any key to place your regions!\n";
		std::system("PAUSE");
		WaitForMyTurnToPlaceMyRegions(name);
		SetRegions(mapBorders);
		WaitForTheRestOfThePlayersToSetTheirRegions();
	}
	std::system("CLS");
	std::cout << "The filling map phase is done!\n";
	std::cout << GetMap();
	std::cout << "Press any key to go to the next phase!\n";
	std::system("PAUSE");
}
void DuelPhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders)
{
	std::system("CLS");
	std::cout << "This is the duel phase! Get ready to attack or to get attacked by your enemies!\n";
	std::system("PAUSE");

	while (true) {
		if (TestIfDuelsAreDone()) {
			break;
		}
		WhatAmI whatAmI = GetWhatAmI(name);
		if (whatAmI == WhatAmI::Attacker) {
			AttackerBehaviour(name, mapBorders);
		}
		else {
			AttackedBehaviour(name, mapBorders);
		}
		std::cout << GetGridQuestion();
		bool iUsedAdvantage = UseAdvantageForCurrentQuestion(name, false);
		SetGridAnswer(name);
		WaitForBouthDuelPlayersToAnswer();
		if (iUsedAdvantage) {
			std::system("CLS");
			std::cout << "You have used one advantage! Choose one region to decrement score from the map.\n";
			std::cout << GetMap();
			SetAdvantageUseRegion(name, mapBorders);
		}
		std::system("CLS");
		bool drawTest = TestIfItWasDrawOnGridQuestion();
		if (drawTest) {
			std::cout << GetNumericalQuestion(name);
			iUsedAdvantage = UseAdvantageForCurrentQuestion(name, true);
			SetNumericalAnswer(name);
			WaitForBouthDuelPlayersToAnswerNumericalQuestion();
		}
		std::system("CLS");
		ShowDuelResult(name);
		std::system("PAUSE");
	}

	std::system("CLS");
	std::cout << "The duel phase is done!\n";
	std::cout << GetMap();
	std::cout << "Press any key to go to the next phase!\n";
	std::system("PAUSE");
}
void ResultsPhase()
{
	std::system("CLS");
	std::cout << GetMap();
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/game_result"});
	std::cout << response.text;
	std::system("PAUSE");
}

void AttackerBehaviour(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders)
{
	std::system("CLS");
	std::cout << "You are the attacker! Choose one region to attack!\n";
	std::cout << GetMap();
	std::cout << "You can attack one of those regions: ";
	GetAttackableRegionsCoordinates();
	SetTheRegionYouWantToAttack(mapBorders);
	std::system("CLS");
}

void AttackedBehaviour(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders)
{
	std::system("CLS");
	std::cout << "You are attacked! Prepare for battle!\n";
	std::system("PAUSE");
	std::system("CLS");
}

void ShowDuelResult(const std::string& name)
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/duel_result/" + name });
	std::cout << response.text;
}
