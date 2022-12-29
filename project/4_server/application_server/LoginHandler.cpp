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
		std::vector<std::tuple<unsigned int, std::string>> rows = m_db.select(sql::columns(&User::GetId, &User::GetPassword), sql::where(sql::c(&User::GetName) == nameIterator->second));
		if (!rows.empty()) {
			if (std::get<1>(rows[0]) != passwordIterator->second) {
				return crow::response(401);
			}
			if (m_loggedUsers.count(nameIterator->second)) {
				return crow::response(402);
			}
			m_loggedUsers.emplace(nameIterator->second, User(std::get<0>(rows[0]), nameIterator->second, passwordIterator->second));
			if (m_loggedUsers.size() == 1) {
				return crow::response(201);
			}
			return crow::response(202);
		}
		unsigned int id = m_db.insert(User(-1, nameIterator->second, passwordIterator->second));
		m_loggedUsers.emplace(nameIterator->second, User(id, nameIterator->second, passwordIterator->second));
		if (m_loggedUsers.size() == 1) {
			return crow::response(201);
		}
		return crow::response(202);
	}
	return crow::response(400);
}