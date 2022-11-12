#include "Question.h"

Question::Question(const std::string& question, const std::vector<std::string>& answers)
	:m_question(question)
	,m_answers(answers)
{
	//empty
}

const std::string& Question::GetQuestion() const
{
	return m_question;
}