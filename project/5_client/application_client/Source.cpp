#include <iostream>

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
LoginState LoginMenu(std::string& name, std::string& password);

std::string GetMap();
std::pair<uint8_t, uint8_t> GetMapBorders();
std::string GetNumericalQuestion(const std::string& name);

void SetGameSize();
void SetNumericalAnswer(const std::string& name);
void SetBase(const std::pair<uint8_t, uint8_t>& borders);
void SetRegions(const std::pair<uint8_t, uint8_t>& borders);

void WaitForAllPlayersToLogin();
void WaitForMyTurnToPlaceMyBase(const std::string& name);
void WaitForTheRestOfThePlayersToSetTheirBase();
void WaitForMyTurnToPlaceMyRegions(const std::string& name);
void WaitForTheRestOfThePlayersToSetTheirRegions();

bool TestIfFillMapPhaseIsDone();

bool LoginPhase(std::string& name, std::string& password);
void ChosingBasePhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);
void FillMapPhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);
void DuelPhase(const std::string& name, const std::pair<uint8_t, uint8_t>& mapBorders);

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

void WaitForAllPlayersToLogin()
{
	while (true) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
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
void WaitForMyTurnToChooseMyOponent();

bool TestIfFillMapPhaseIsDone()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/map_is_full" });
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

}
