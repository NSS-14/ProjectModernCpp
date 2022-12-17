#include "Question.h"

Question::Question(const Question& question)
{
	m_question = question.m_question;
	m_answers = question.m_answers;
}

Question::Question(unsigned int id, const std::string& question, const std::string& answer)
	:m_id(id), m_question(question), m_answers(1)
{
	SetAnswer(answer);
}

Question::Question(unsigned int id, const std::string& question, const std::vector<std::string>& answers)
	:m_id(id)
	,m_question(question)
	,m_answers(answers)
{
	//empty
}

const std::string& Question::GetQuestion() const
{
	return m_question;
}


std::string Question::GetAnswer() const
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

unsigned int Question::GetId() const
{
	return m_id;
}

void Question::SetQuestion(const std::string& question)
{
	m_question = question;
}

void Question::SetAnswers(const std::vector<std::string>& answers)
{
	m_answers = answers;
}

void Question::SetAnswer(const std::string& answer)
{
	m_answers[0] = answer;
}

void Question::SetId(unsigned int id)
{
	m_id = id;
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

std::ostream& operator<<(std::ostream& out, const Question& question)
{
	out << question.m_question << "\n";
	std::vector<std::string> answers = question.GetAnswers();
	for (int i = 0; i < 4; ++i) {
		out << answers[i] << " ";
	}
	out << "\n";
	return out;
}

WrongAnswer::WrongAnswer(const WrongAnswer& wrongAnswer)
{
	*this = wrongAnswer;
}

WrongAnswer::WrongAnswer(unsigned int id, unsigned int questionId, const std::string& wrongAnswer)
	: m_id(id), m_questionId(questionId), m_wrongAnswer(wrongAnswer)
{
	/* EMPTY */
}

WrongAnswer& WrongAnswer::operator=(const WrongAnswer& wrongAnswer)
{
	m_id = wrongAnswer.m_id;
	m_questionId = wrongAnswer.m_questionId;
	m_wrongAnswer = wrongAnswer.m_wrongAnswer;
}

bool WrongAnswer::operator==(const WrongAnswer& wrongAnswer)
{
	return m_id == wrongAnswer.m_id && m_questionId == wrongAnswer.m_questionId && m_wrongAnswer == wrongAnswer.m_wrongAnswer;
}

unsigned int WrongAnswer::GetId() const
{
	return m_id;
}

unsigned int WrongAnswer::GetQuestionId() const
{
	return m_questionId;
}

const std::string& WrongAnswer::GetWrongAnswer() const
{
	return m_wrongAnswer;
}

void WrongAnswer::SetId(unsigned int id)
{
	m_id = id;
}

void WrongAnswer::SetQuestionId(unsigned int id)
{
	m_questionId = id;
}

void WrongAnswer::SetWrongAnswer(const std::string& wrongAnswer)
{
	m_wrongAnswer = wrongAnswer;
}
