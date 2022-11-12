#pragma once
#include<string>
#include<vector>

class Question
{
public:
	Question(const std::string& question, const std::vector<std::string>& answers);
	const std::string& GetQuestion() const;
	const std::string& GetAnswer() const;
	const std::vector <std::string> GetAnswers() const;

private:
	std::string m_question;
	std::vector <std::string> m_answers;

};

