#pragma once
#include<string>
#include<vector>
#include<set>
#include <iostream>

class WrongAnswer {
public:
	WrongAnswer() = default;
	WrongAnswer(const WrongAnswer &wrongAnswer);
	WrongAnswer(const std::string& wrongAnswer);

public:
	unsigned int GetId() const;
	unsigned int GetQuestionId() const;
	const std::string& GetWrongAnswer() const;

private:
	void SetId(unsigned int id);
	void SetQuestionId(unsigned int id);
	void SetWrongAnswer(const std::string& wrongAnswer);

private:
	unsigned int m_id;
	unsigned int m_questionId;
	std::string wrongAnswer;
};

class Question
{
public:
	Question() = default;
	Question(const Question& question);
	Question(const std::string& question, const std::vector<std::string>& answers);

	unsigned int GetId() const;
	const std::string& GetQuestion() const;
	std::string GetAnswer() const;
	std::vector<std::string> GetAnswers() const;

	void SetId(unsigned int id);
	void SetQuestion(const std::string& question);
	void SetAnswers(const std::vector <std::string>& answers);
	void SetAnswer(const std::string& answer);


	friend std::ostream& operator << (std::ostream& out, const Question& question);

private:
	unsigned int m_id;
	std::string m_question;
	std::vector <std::string> m_answers;
	void GenerateRandomIndexes(std::vector<int>& vector) const;

};

