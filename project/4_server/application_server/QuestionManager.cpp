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
		Question question(questionString, answers);
		answers.clear();
		if (type)
			m_numericalQuestions.push_back(question);
		else
			m_gridQuestions.push_back(question);
	}
	in.close();
}
