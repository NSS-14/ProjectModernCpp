#include "DBBehaviour.h"

LoginHandler::LoginHandler(Storage& storage)
	:m_db(storage)
{
	/*EMPTY*/
}

crow::response LoginHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = ParseUrlArgs(req.body);
	auto end = bodyArgs.end();
	auto nameIter = bodyArgs.find("name");
	auto passwordIter = bodyArgs.find("password");

	if (nameIter != end && passwordIter != end) {
		User user(-1, nameIter->second, passwordIter->second);
		m_db.insert(user);
	}

	return crow::response(201);
}
