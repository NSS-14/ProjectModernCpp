#include <iostream>

#include "LoginHandler.h"

int main()
{
	crow::SimpleApp app;

	LoginHandler loginHandler;

	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(loginHandler);

	app.port(18080).multithreaded().run();

	return 0;
}