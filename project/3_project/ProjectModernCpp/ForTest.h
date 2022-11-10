#pragma once

#include <string>

class ForTest
{
public:
	ForTest() = default;
	ForTest(const std::string& question, const std::string& answer);

	const std::string& GetQuestion() const;
	const std::string& GetAnswer() const;

private:
	std::string m_question;
	std::string m_answer;
};

