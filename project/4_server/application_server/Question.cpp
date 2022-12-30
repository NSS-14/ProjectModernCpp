#include "Question.h"

// WrongAnswer class implementation:
WrongAnswer::WrongAnswer(const WrongAnswer& wrongAnswer)
{
	*this = wrongAnswer;
}
WrongAnswer::WrongAnswer(WrongAnswer&& wrongAnswer) noexcept
{
	*this = std::move(wrongAnswer);
}
WrongAnswer::WrongAnswer(unsigned int id, unsigned int questionId, const std::string& wrongAnswer)
	: m_id(id)
	, m_questionId(questionId)
	, m_wrongAnswer(wrongAnswer)
{
	/* EMPTY */
}

WrongAnswer& WrongAnswer::operator=(const WrongAnswer& wrongAnswer)
{
	m_id = wrongAnswer.m_id;
	m_questionId = wrongAnswer.m_questionId;
	m_wrongAnswer = wrongAnswer.m_wrongAnswer;
	return *this;
}
WrongAnswer& WrongAnswer::operator=(WrongAnswer&& wrongAnswer) noexcept
{
	m_id = wrongAnswer.m_id;
	m_questionId = wrongAnswer.m_questionId;
	m_wrongAnswer = std::move(wrongAnswer.m_wrongAnswer);
	return *this;
}
bool WrongAnswer::operator==(const WrongAnswer& wrongAnswer)
{
	return m_id == wrongAnswer.m_id
		&& m_questionId == wrongAnswer.m_questionId
		&& m_wrongAnswer == wrongAnswer.m_wrongAnswer;
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

// Question class implementation:
Question::Question(const Question& question)
{
	*this = question;
}
Question::Question(Question&& question) noexcept
{
	*this = std::move(question);
}
Question::Question(unsigned int id, const std::string& question, const std::string& answer, bool isNumerical)
	: m_id(id)
	, m_question(question)
	, m_answers(1)
	, m_isNumerical(isNumerical)
{
	SetAnswer(answer);
}
Question::Question(unsigned int id, const std::string& question, const std::vector<std::string>& answers, bool isNumerical)
	: m_id(id)
	, m_question(question)
	, m_answers(answers)
	, m_isNumerical(isNumerical)
{
	/* EMPTY */
}

Question& Question::operator=(const Question& question)
{
	m_id = question.m_id;
	m_question = question.m_question;
	m_answers = question.m_answers;
	m_isNumerical = question.m_isNumerical;
	return *this;
}
Question& Question::operator=(Question&& question) noexcept
{
	m_id = question.m_id;
	m_question = std::move(question.m_question);
	m_answers = std::move(question.m_answers);
	m_isNumerical = question.m_isNumerical;
	return *this;
}
bool Question::operator==(const Question& question)
{
	return m_id == question.m_id
		&& m_question == question.m_question
		&& m_answers == question.m_answers
		&& m_isNumerical == question.m_isNumerical;
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

unsigned int Question::GetId() const
{
	return m_id;
}
const std::string& Question::GetQuestion() const
{
	return m_question;
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
std::string Question::GetAnswer() const
{
	return m_answers[0];
}
std::vector<std::string> Question::GetWrongAnswers() const
{
	std::vector<std::string> wrongAnswers;
	wrongAnswers.reserve(m_answers.size() - 1);

	for (size_t i = 1; i < m_answers.size(); ++i) {
		wrongAnswers.push_back(m_answers[i]);
	}
	return wrongAnswers;
}
bool Question::GetType() const
{
	return m_isNumerical;
}

void Question::SetId(unsigned int id)
{
	m_id = id;
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
void Question::SetType(bool isNumerical)
{
	m_isNumerical = isNumerical;
}

void Question::AddWrongAnswer(const std::string& wrongAnswer)
{
	m_answers.push_back(wrongAnswer);
}
void Question::AppendWrongAnswers(const std::vector<std::string>& wrongAnswers)
{
	m_answers.insert(std::end(m_answers), std::begin(wrongAnswers), std::end(wrongAnswers));
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
