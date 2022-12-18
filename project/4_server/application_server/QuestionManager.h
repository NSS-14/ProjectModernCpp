#pragma once
#include<list>
#include"Question.h"
#include<fstream>
#include"DBBehaviour.h"

class QuestionManager
{
public:
	QuestionManager()=default;
	QuestionManager(const QuestionManager& questionManager);
	QuestionManager(QuestionManager&& questionManager);
	QuestionManager(std::ifstream& in);
	QuestionManager(std::string path);

public:
	QuestionManager& operator =(const QuestionManager& questionManager);
	QuestionManager& operator =(QuestionManager&& questionManager);

public:
	Question GetNumericalQuestion();
	Question GetGridQuestion();

public:
	void ReadFile(std::ifstream& in);
	void ReadDataBase(Storage& db);
	void PopulateDataBase(Storage& db) const;

private:
	std::list<Question> m_numericalQuestions;
	std::list<Question> m_gridQuestions;
};

