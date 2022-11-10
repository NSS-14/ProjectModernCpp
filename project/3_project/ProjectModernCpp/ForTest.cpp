#include "ForTest.h"

ForTest::ForTest(const std::string& question, const std::string& answer)
	: m_question(question)
	, m_answer(answer)
{
	/*EMPTY*/
}

const std::string& ForTest::GetQuestion() const
{
	return m_question;
}

const std::string& ForTest::GetAnswer() const
{
	return m_answer;
}
