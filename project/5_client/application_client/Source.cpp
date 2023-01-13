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

int main()
{
	std::string name;
	std::string password;
	std::pair<uint8_t, uint8_t> mapBorders;
	LoginState loginState = LoginMenu(name, password);
	if (loginState == LoginState::Error) {
		return 1;
	}
	if (loginState == LoginState::Host) {
		SetGameSize();
	}
	while (true) {
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/ready" });
		if (response.status_code == 200) {
			break;
		}
	}

	std::system("CLS");
	std::cout << "Game ready!\n";
	std::cout << GetMap();
	std::system("PAUSE");

	std::system("CLS");
	std::cout << GetNumericalQuestion(name);
	SetNumericalAnswer(name);
	mapBorders = GetMapBorders();
	std::system("PAUSE");

	std::system("CLS");
	std::string lastMap = GetMap();
	std::cout << lastMap;
	std::cout << "\nWait for your turn to place your base.";
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/is_my_turn/" + name });
		if (response.status_code == 200) {
			break;
		}
		if (lastMap != GetMap()) {
			std::system("CLS");
			lastMap = GetMap();
			std::cout << lastMap;
			std::cout << "\nWait for your turn to place your base.";
		}
	}

	std::system("CLS");
	std::cout << GetMap();
	SetBase(mapBorders);
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/phase_done" });
		if (response.status_code == 200) {
			break;
		}
		if (lastMap != GetMap()) {
			std::system("CLS");
			lastMap = GetMap();
			std::cout << lastMap;
			std::cout << "\nWait for all players to set their base.";
		}
	}
	std::system("CLS");
	std::cout << GetMap();
	std::cout << "\nThe chosing base phase is done!\n";
	std::system("PAUSE");

	while (true) {
		std::system("CLS");
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/map_is_full" });
		if (response.status_code == 200) {
			break;
		}
		std::cout << GetNumericalQuestion(name);
		SetNumericalAnswer(name);
		std::system("PAUSE");

		std::system("CLS");
		lastMap = GetMap();
		std::cout << lastMap;
		std::cout << "\nWait for your turn to place your regions.";
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/is_my_turn/" + name });
			if (response.status_code == 200) {
				break;
			}
			if (lastMap != GetMap()) {
				std::system("CLS");
				lastMap = GetMap();
				std::cout << lastMap;
				std::cout << "\nWait for your turn to place your regions.";
			}
		}
		std::system("CLS");
		std::cout << GetMap();
		SetRegions(mapBorders);
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/phase_done" });
			if (response.status_code == 200) {
				break;
			}
			if (lastMap != GetMap()) {
				std::system("CLS");
				lastMap = GetMap();
				std::cout << lastMap;
				std::cout << "\nWait for all players to set their regions.";
			}
		}
	}
	std::system("CLS");
	std::cout << GetMap();
	std::cout << "\nThe filling map phase is done!\n";
	std::system("PAUSE");

	return 0;
}

LoginState LoginMenu(std::string& name, std::string& password) {

	while (true) {
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

void SetGameSize()
{
	std::string gameSize;
	std::regex digitIntervalRegex("[2-4]");

	while (true) {
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
		std::cout << "Wait for the rest of players.";
	}
}
void SetBase(const std::pair<uint8_t, uint8_t>& borders)
{
	std::pair<std::string, std::string> answer;
	std::regex number("[0-9]|([1-9][0-9]+)");

	while (true) {
		std::cout << "Insert your base coordinates: ";
		std::cin >> answer.first >> answer.second;
		if (!std::regex_match(answer.first, number) || !std::regex_match(answer.second, number)) {
			std::cout << "Your input is not a number. Try again!\n";
			continue;
		}
		if (std::stoi(answer.first) >= borders.first || std::stoi(answer.second) >= borders.second ) {
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
		std::system("CLS");
		std::cout << GetMap();
		std::cout << "You shoud place " << numberOfRegionsIcanPlace << " regions!\n";
		std::cout << "Insert your region (" << i + 1 << ") coordinates: ";
		std::cin >> answer.first >> answer.second;
		if (!std::regex_match(answer.first, number) || !std::regex_match(answer.second, number)) {
			std::cout << "Your input is not a number. Try again!\n";
			--i;
			continue;
		}
		if (std::stoi(answer.first) >= borders.first || std::stoi(answer.second) >= borders.second) {
			std::cout << "Your input is out of bounds. Try again!\n";
			--i;
			continue;
		}
		auto response = cpr::Get(cpr::Url{ "http://localhost:18080/set_region/" + answer.first + answer.second });
		if (response.status_code != 200) {
			std::cout << "The place is already ocupied. Try another place!\n";
			--i;
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
