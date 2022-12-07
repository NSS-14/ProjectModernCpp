#include <filesystem>
#include <iostream>
#include <memory>
#include"DBBehaviour.h"
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;


int main()
{
	const std::string dbFile = "database.sqlite";
	Storage db = CreateStorage(dbFile);
	db.sync_schema();

	crow::SimpleApp app;
	
	CROW_ROUTE(app, "/login/<string>/<string>")([&db](const crow::request & req, const std::string & name, const std::string & password)
	{
		User user(name, password);
		db.replace(user);

		return crow::response(200);
	});

	app.port(18080).multithreaded().run();

	return 0;
}