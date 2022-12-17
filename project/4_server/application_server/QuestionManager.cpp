#include "QuestionManager.h"


QuestionManager::QuestionManager(std::ifstream& in)
{
	ReadFile(in);
}

QuestionManager::QuestionManager(std::string path)
{
	std::ifstream in(path);
	ReadFile(in);
}

Question QuestionManager::GetNumericalQuestion()
{
	unsigned int randomPosition = rand() % m_numericalQuestions.size();
	std::list<Question>::const_iterator it = m_numericalQuestions.begin();
	std::advance(it, randomPosition);

	Question tmp(*it);
	m_numericalQuestions.erase(it);
	return tmp;
}

Question QuestionManager::GetGridQuestion()
{
	unsigned int randomPosition = rand() % m_gridQuestions.size();
	std::list<Question>::const_iterator it = m_gridQuestions.begin();
	std::advance(it, randomPosition);

	Question tmp(*it);
	m_gridQuestions.erase(it);
	return tmp;
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
		std::getline(in,questionString);
		std::getline(in, questionString);
		std::string str;
		in >> numberOfAnswers;
		std::getline(in, str);
		for (int i = 0; i < numberOfAnswers; i++)
		{
			std::string answer;
			std::getline(in,answer);
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

	int totalNumberOfQuestionsFromDB = db.count<Question>();
	for (int i = 1; i <= totalNumberOfQuestionsFromDB; ++i)
	{
		auto rows1= db.select(sql::columns(&Question::GetQuestion, &Question::GetAnswer, &Question::GetType), sql::where(sql::c(&Question::GetId)==i));
		const auto& tuple = rows1[0];

		Question q(i,std::get<0>(tuple),std::get<1>(tuple), std::get<2>(tuple));
		
		auto rows2 = db.select(&WrongAnswer::GetWrongAnswer, sql::where(sql::c(&WrongAnswer::GetQuestionId) == i));
		q.AppendWrongAnswers(rows2);

		if (q.GetType())
			m_numericalQuestions.push_back(q);
		else
			m_gridQuestions.push_back(q);
	}
}
