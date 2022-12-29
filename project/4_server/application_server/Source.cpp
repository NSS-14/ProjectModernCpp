#include <iostream>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "DBBehaviour.h"
#include "LoginHandler.h"

namespace sql = sqlite_orm;

int main()
{
	using namespace sqlite_orm;
	const std::string dbFile = "database.sqlite";
	Storage db = CreateStorage(dbFile);
	db.sync_schema();

	crow::SimpleApp app;

	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(LoginHandler(db));

	app.port(18080).multithreaded().run();

	return 0;
}