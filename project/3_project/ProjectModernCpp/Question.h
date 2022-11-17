#pragma once
#include<string>
#include<vector>
#include<set>
#include <iostream>

class Question
{
public:
	Question() = default;
	Question(const Question& question);
	Question(const std::string& question, const std::vector<std::string>& answers);
	const std::string& GetQuestion() const;
	const std::string& GetAnswer() const;
	std::vector<std::string> GetAnswers() const;
	friend std::ostream& operator << (std::ostream& out, const Question& question);

private:
	std::string m_question;
	std::vector <std::string> m_answers;
	void GenerateRandomIndexes(std::vector<int>& vector) const;
};

