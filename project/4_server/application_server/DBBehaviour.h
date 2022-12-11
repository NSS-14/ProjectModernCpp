#pragma once
#include<string>
#include<vector>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;
#include "User.h"
#include "UtilityFunctions.h"
//#include "Question.h"

inline auto CreateStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table(
			"User",
			sql::make_column("id", &User::GetId, &User::SetId, sql::autoincrement(), sql::primary_key()),
			sql::make_column("name", &User::GetName, &User::SetName),
			sql::make_column("password", &User::GetPassword, &User::SetPassword)
		)//,
		/*sql::make_table(
			"Question",
			sql::make_column("question", &Question::GetQuestion, &Question::SetQuestion, sql::primary_key()),
			sql::make_column("answer", &Question::GetAnswer, &Question::SetAnswer),
			sql::make_column("answers", &Question::GetAnswers, &Question::SetAnswers)
		)*/
		);
}
using Storage = decltype(CreateStorage(""));

class LoginHandler {
public:
	LoginHandler(Storage& storage);
	crow::response operator()(const crow::request& req) const;

private:
	Storage& m_db;
};
