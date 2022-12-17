#pragma once
#include<list>
#include"Question.h"
#include<fstream>
#include"DBBehaviour.h"

class QuestionManager
{
public:
	QuestionManager()=default;
	QuestionManager(std::ifstream& in);
	QuestionManager(std::string path);

public:
	Question GetNumericalQuestion();
	Question GetGridQuestion();

public:
	void ReadFile(std::ifstream& in);
	void ReadDataBase( Storage& db);
	void PopulateDataBase(Storage& db);

private:
	std::list<Question> m_numericalQuestions;
	std::list<Question> m_gridQuestions;
};
