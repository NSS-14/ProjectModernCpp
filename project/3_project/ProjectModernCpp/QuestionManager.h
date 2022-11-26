#pragma once
#include<list>
#include"Question.h"
#include<fstream>

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


private:
	std::list<Question> m_numericalQuestions;
	std::list<Question> m_gridQuestions;
};

