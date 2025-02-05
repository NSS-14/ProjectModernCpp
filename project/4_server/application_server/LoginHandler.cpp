#include "LoginHandler.h"
#include "UtilityFunctions.h"
#include "StorageManager.h"

LoginHandler::LoginHandler(ReadyManager& readyManager)
	: m_readyManager(readyManager)
{
	/* EMPTY */
}

crow::response LoginHandler::operator()(const crow::request& request) const
{
	using namespace sqlite_orm;
	using Dictionary = std::unordered_map<std::string, std::string>;
	using IdPasswordVector = std::vector<std::tuple<unsigned int, std::string>>;

	Storage& db = StorageManager::GetInstance().GetStorage();
	Dictionary parsedArguments = ParseUrlArguments(request.body);
	Dictionary::const_iterator end = parsedArguments.end();
	Dictionary::const_iterator nameIterator = parsedArguments.find("name");
	Dictionary::const_iterator passwordIterator = parsedArguments.find("password");

	if (nameIterator == end || passwordIterator == end) {
		return crow::response(400);
	}
	if (m_readyManager.GetOnlinePlayers()) {
		if (m_readyManager.GetOnlinePlayers() == m_readyManager.GetDesiredNumberOfPlayers()) {
			return crow::response(500);
		}
	}
	IdPasswordVector rows = db.select(sql::columns(&User::GetId, &User::GetPassword), sql::from<User>(), sql::where(sql::c(&User::GetName) == nameIterator->second));
	if (!rows.empty()) {
		unsigned int id = std::get<0>(rows[0]);
		const std::string& realPassword = std::get<1>(rows[0]);
		if (realPassword != passwordIterator->second) {
			return crow::response(401);
		}
		if (ContainsUserWithId(m_readyManager.GetUsers(), id)) {
			return crow::response(403);
		}
		m_readyManager.AddUser(id, nameIterator->second, passwordIterator->second);
		if (m_readyManager.GetOnlinePlayers() == 1) {
			return crow::response(201);
		}
		return crow::response(202);
	}
	unsigned int id = db.insert(User(0, nameIterator->second, passwordIterator->second));
	m_readyManager.AddUser(id, nameIterator->second, passwordIterator->second);
	if (m_readyManager.GetOnlinePlayers() == 1) {
		return crow::response(201);
	}
	return crow::response(202);
}