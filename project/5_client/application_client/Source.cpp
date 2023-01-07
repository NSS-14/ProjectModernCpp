#include <iostream>

#include <cpr/cpr.h>
#include <crow.h>
#include <regex>

enum class LoginState {
	Host,
	Wait,
	Error
};
LoginState LoginMenu();
void SetGameSize();


int main()
{
	LoginState loginState = LoginMenu();
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
	std::cout << "Game ready!";
	std::system("PAUSE");

	auto response = cpr::Get(cpr::Url{ "http://localhost:18080/map" });
	std::cout << response.text;
	std::system("PAUSE");

	return 0;
}

LoginState LoginMenu() {
	std::string name;
	std::string password;

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

	while (true) {
		std::system("CLS");
		std::cout << "Insert the game size: ";
		std::cin >> gameSize;
		std::regex digitIntervalRegex("[2-4]");
		if (!std::regex_match(gameSize, digitIntervalRegex)) {
			std::cout << "You have to insert a number between 2 and 4!\n";
			std::system("PAUSE");
			continue;
		}
		break;
	}
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/host/set-size" },
		cpr::Payload{
			{ "size", gameSize }
		}
	);
	if (response.status_code == 200) {
		std::cout << "The game size is " << gameSize << '.' << std::endl;
	}
}
