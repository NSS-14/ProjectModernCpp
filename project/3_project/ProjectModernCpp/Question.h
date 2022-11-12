#pragma once
#include<string>
#include<vector>

class Question
{
public:
	Question(std::string question, std::vector<std::string> answers);
	const std::string& GetQuestion() const;

private:
	std::string m_question;
	std::vector <std::string> m_answers;

};

