#include "QuestionManager.h"
#include <unordered_set>
#include <ctime>

QuestionManager::QuestionManager()
	: m_nextNumericalQuestionIndex(0)
	, m_nextGridQuestionIndex(0)
{
	//EMPTY
}
QuestionManager::QuestionManager(const QuestionManager& questionManager)
{
	*this = questionManager;
}
QuestionManager::QuestionManager(QuestionManager&& questionManager) noexcept
{
	*this = std::move(questionManager);
}

QuestionManager& QuestionManager::operator=(const QuestionManager& questionManager)
{
	m_gridQuestions = questionManager.m_gridQuestions;
	m_numericalQuestions = questionManager.m_numericalQuestions;
	return *this;
}
QuestionManager& QuestionManager::operator=(QuestionManager&& questionManager) noexcept
{
	m_gridQuestions = std::move(questionManager.m_gridQuestions);
	m_numericalQuestions = std::move(questionManager.m_numericalQuestions);
	return *this;
}

const Question& QuestionManager::GetNumericalQuestion() const
{
	return m_numericalQuestions[m_nextNumericalQuestionIndex++];
}
const Question& QuestionManager::GetGridQuestion() const
{
	return m_gridQuestions[m_nextGridQuestionIndex++];
}

void QuestionManager::ReadDataBase(Storage& db)
{
	using namespace sqlite_orm;
	srand(std::time(0));
	std::unordered_set<unsigned int> usedIds;
	int numberOfQuestions = db.count<Question>();
	for (int i = 0; i < numberOfQuestions; ++i)
	{
		unsigned int randomId = 1 + rand() % numberOfQuestions;
		if (usedIds.count(randomId))
		{
			--i;
			continue;
		}
		usedIds.insert(randomId);
		auto row=db.select(sql::columns(&Question::GetId, &Question::GetQuestion, &Question::GetAnswer, &Question::GetType), sql::where(sql::c(&Question::GetId)==randomId));
		Question q(std::get<0>(row[0]), std::get<1>(row[0]), std::get<2>(row[0]), std::get<3>(row[0]));
		auto wrongAnswers = db.select(&WrongAnswer::GetWrongAnswer, sql::where(sql::c(&WrongAnswer::GetQuestionId) == q.GetId()));
		q.AppendWrongAnswers(wrongAnswers);

		if (q.GetType()) {
			m_numericalQuestions.emplace_back(std::move(q));
			continue;
		}
		m_gridQuestions.emplace_back(std::move(q));
	}
}
