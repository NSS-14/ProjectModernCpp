#include "LoginHandler.h"

#include "UtilityFunctions.h"

LoginHandler::LoginHandler(Storage& storage)
	:m_db(storage)
{
	/* EMPTY */
}

crow::response LoginHandler::operator()(const crow::request& request) const
{
	using namespace sqlite_orm;
	using Dictionary = std::unordered_map<std::string, std::string>;

	Dictionary parsedArguments = ParseUrlArguments(request.body);
	Dictionary::const_iterator end = parsedArguments.end();
	Dictionary::const_iterator nameIterator = parsedArguments.find("name");
	Dictionary::const_iterator passwordIterator = parsedArguments.find("password");

	if (nameIterator != end && passwordIterator != end) {
		std::vector<std::string> rows = m_db.select(sql::column<User>(&User::GetPassword), sql::where(sql::c(&User::GetName) == nameIterator->second));
		if (!rows.empty()) {
			if (rows[0] != passwordIterator->second) {
				return crow::response(401);
			}
			return crow::response(200);
		}
		m_db.insert(User(-1, nameIterator->second, passwordIterator->second));
		return crow::response(201);
	}
	return crow::response(400);
}