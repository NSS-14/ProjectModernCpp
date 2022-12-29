#include "AnswerHandler.h"

AnswerHandler::AnswerHandler()
{
}

crow::response AnswerHandler::operator()(const crow::request& request) const
{
	return crow::response();
}
