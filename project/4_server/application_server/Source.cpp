#include <iostream>

#include "LoginHandler.h"

namespace sql = sqlite_orm;

int main()
{
	crow::SimpleApp app;

	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(LoginHandler(StorageManager::GetInstance().GetStorage()));

	app.port(18080).multithreaded().run();

	return 0;
}