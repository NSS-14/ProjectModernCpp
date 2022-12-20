#include <iostream>
#include <numeric>
#include <sstream>
#include <regex>
#include <string>

#include <cpr/cpr.h>
#include <crow.h>

void LoginMenu();

int main()
{
	LoginMenu();

	return 0;
}

void LoginMenu() {
	std::string name;
	std::string password;

	while (true) {
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
		if (response.status_code == 200 || response.status_code == 201) {
			std::cout << "Your account was successfully logged on!\n";
			std::system("PAUSE");
			return;
		}
		std::cout << "Your account was not logged on becouse the inserted password is incorect.\n";
		std::system("PAUSE");
	}
}