#pragma once
#include <sqlite_orm/sqlite_orm.h>

#include "User.h"
#include "Question.h"

namespace sql = sqlite_orm;

inline auto CreateStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table(
			"User",
			sql::make_column("id", &User::GetId, &User::SetId, sql::autoincrement(), sql::primary_key()),
			sql::make_column("name", &User::GetName, &User::SetName),
			sql::make_column("password", &User::GetPassword, &User::SetPassword)
		),
		sql::make_table(
			"Question",
			sql::make_column("id", &Question::GetId, &Question::SetId, sql::autoincrement(), sql::primary_key()),
			sql::make_column("question", &Question::GetQuestion, &Question::SetQuestion),
			sql::make_column("answer", &Question::GetAnswer, &Question::SetAnswer),
			sql::make_column("type", &Question::GetType, &Question::SetType)
		),
		sql::make_table(
			"WrongAnswer",
			sql::make_column("id", &WrongAnswer::GetId, &WrongAnswer::SetId, sql::autoincrement(), sql::primary_key()),
			sql::make_column("questionId", &WrongAnswer::GetQuestionId, &WrongAnswer::SetQuestionId),
			sql::make_column("wrongAnswer", &WrongAnswer::GetWrongAnswer, &WrongAnswer::SetWrongAnswer)
		)
		);
}
using Storage = decltype(CreateStorage(""));