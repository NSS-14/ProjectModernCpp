#include <filesystem>
#include <iostream>
#include <memory>
#include"DBBehaviour.h"
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;


int main()
{
	using namespace sqlite_orm;
	const std::string dbFile = "database.sqlite";
	Storage db = CreateStorage(dbFile);
	db.sync_schema();

	//crow::SimpleApp app;

	/*CROW_ROUTE(app, "/login/<string>/<string>")([&db](const crow::request& req, const std::string& name, const std::string& password) {
		using namespace sqlite_orm;
		User user(1, name, password);

		auto rows = db.select(&User::GetName, sql::where(sql::c(&User::GetName) == name));
		if(rows.empty())
			db.insert(user);

	    return crow::response(200);
	});

	auto& loginPut = CROW_ROUTE(app, "/login")
		.methods(crow::HTTPMethod::PUT);
	loginPut(LoginHandler(db));*/

	std::vector<std::string> answ = {"abc", "1234"};
	Question q(1, "ABC??", answ);
	db.insert(q);

	for (const auto& answer : answ) {
		WrongAnswer wrAns(1, 1, answer);
		db.insert(wrAns);
	}

	auto rows = db.select(&WrongAnswer::GetWrongAnswer, sql::where(sql::c(&WrongAnswer::GetQuestionId) == 1));

	for (const auto& answer : rows) {
		std::cout << answer << "\n";
	}

	//app.port(18080).multithreaded().run();

	return 0;
}