#include <iostream>

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
		if (response.status_code == 200 || response.status_code == 201) {
			std::cout << "Your account was successfully logged in!\n";
			std::system("PAUSE");
			return;
		}
		if (response.status_code != 400 && response.status_code != 401 && response.status_code != 0) {
			std::cout << "There was an error on the server. We are sorry!\nError code: " << response.status_code << '\n';
			std::system("PAUSE");
			return;
		}
		if (response.status_code == 0) std::cout << "The server is offline. Try again later!";
		else std::cout << "Your account was not logged on becouse the inserted password is incorect.";
		std::cout << "\nPress 1 and enter to retry the login operation.\n";
		int choice; std::cin >> choice;
		if (choice != 1) {
			return;
		}
	}
}