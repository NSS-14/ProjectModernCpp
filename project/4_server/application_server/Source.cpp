#include <iostream>
#include "QuestionManager.h"
#include "LoginHandler.h"

int main()
{
	crow::SimpleApp app;

	LoginHandler loginHandler;

	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(loginHandler);

	QuestionManager qm;
	qm.ReadDataBase(StorageManager::GetInstance().GetStorage());

	app.port(18080).multithreaded().run();

	return 0;
}