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

const std::string& Question::GetAnswer() const
{
	return m_answers[0];
}

std::vector<std::string> Question::GetAnswers() const
{
	std::vector<int> indexes;
	GenerateRandomIndexes(indexes);
	std::vector<std::string> fourAnswers;
	fourAnswers.reserve(4);
	for (int i = 0; i < 4; i++)
	{
		fourAnswers.push_back(m_answers[indexes[i]]);
	}
	return fourAnswers;
}

void Question::GenerateRandomIndexes(std::vector<int>& vector) const
{
	vector.reserve(4);
	std::set<int> set;
	int position;
	while (true)
	{
		position = rand() % m_answers.size();
		if (set.size() == 4)
			break;
		if (position == 0)
			continue;
		set.insert(position);
	}
	for (const auto& element : set)
	{
		vector.push_back(element);
	}
	vector[rand() % 4] = 0;
}
