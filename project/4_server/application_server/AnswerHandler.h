#pragma once

#include <crow.h>

class AnswerHandler {
public:
		AnswerHandler();
		crow::response operator()(const crow::request& request) const;
};