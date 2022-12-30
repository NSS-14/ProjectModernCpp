#include "QuestionManager.h"


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

QuestionManager::QuestionManager(QuestionManager&& questionManager)
{
	*this = std::move(questionManager);
}

QuestionManager::QuestionManager(std::ifstream& in)
{
	ReadFile(in);
}

QuestionManager::QuestionManager(std::string path)
{
	std::ifstream in(path);
	ReadFile(in);
}

QuestionManager& QuestionManager::operator=(const QuestionManager& questionManager)
{
	m_gridQuestions = questionManager.m_gridQuestions;
	m_numericalQuestions = questionManager.m_numericalQuestions;
	return *this;
}

QuestionManager& QuestionManager::operator=(QuestionManager&& questionManager)
{
	m_gridQuestions = std::move(questionManager.m_gridQuestions);
	m_numericalQuestions = std::move(questionManager.m_numericalQuestions);
	return *this;
}

Question QuestionManager::GetNumericalQuestion()
{
	return m_numericalQuestions[m_nextNumericalQuestionIndex++];
}

Question QuestionManager::GetGridQuestion()
{
	return m_gridQuestions[m_nextGridQuestionIndex++];
}

void QuestionManager::ReadFile(std::ifstream& in)
{
	bool type;
	std::string questionString;
	int numberOfAnswers;
	std::vector<std::string> answers;

	while (!in.eof())
	{
		in >> type;
		std::getline(in, questionString);
		std::getline(in, questionString);
		std::string str;
		in >> numberOfAnswers;
		std::getline(in, str);
		for (int i = 0; i < numberOfAnswers; i++)
		{
			std::string answer;
			std::getline(in, answer);
			answers.push_back(answer);
		}
		Question question(1, questionString, answers, type);
		answers.clear();
		if (type)
			m_numericalQuestions.push_back(question);
		else
			m_gridQuestions.push_back(question);
	}
	in.close();
}

void QuestionManager::ReadDataBase(Storage& db)
{
	using namespace sqlite_orm;

	auto rows = db.select(sql::columns(&Question::GetId, &Question::GetQuestion, &Question::GetAnswer, &Question::GetType));
	for (const auto& row : rows) {
		Question q(std::get<0>(row), std::get<1>(row), std::get<2>(row), std::get<3>(row));
		auto wrongAnswers = db.select(&WrongAnswer::GetWrongAnswer, sql::where(sql::c(&WrongAnswer::GetQuestionId) == q.GetId()));
		q.AppendWrongAnswers(wrongAnswers);

		if (q.GetType()) {
			m_numericalQuestions.push_back(q);
			continue;
		}
		m_gridQuestions.push_back(q);
	}
}

void QuestionManager::PopulateDataBase(Storage& db) const
{
	using namespace sqlite_orm;

	for (const auto& question : m_gridQuestions)
	{
		auto rows = db.select(&Question::GetQuestion, sql::where(sql::c(&Question::GetQuestion) == question.GetQuestion()));
		if (rows.size()) {
			continue;
		}

		int id = db.insert(question);
		std::vector<std::string> wrongAnswers = question.GetWrongAnswers();
		for (const auto& wrongAnswer : wrongAnswers)
		{
			WrongAnswer wa(1, id, wrongAnswer);
			db.insert(wa);
		}
	}
	for (const auto& question : m_numericalQuestions)
	{
		auto rows = db.select(&Question::GetQuestion, sql::where(sql::c(&Question::GetQuestion) == question.GetQuestion()));
		if (rows.size()) {
			continue;
		}

		int id = db.insert(question);
		std::vector<std::string> wrongAnswers = question.GetWrongAnswers();
		for (const auto& wrongAnswer : wrongAnswers)
		{
			WrongAnswer wa(1, id, wrongAnswer);
			db.insert(wa);
		}
	}
}
