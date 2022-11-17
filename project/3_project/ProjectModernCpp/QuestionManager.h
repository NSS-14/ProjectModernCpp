#pragma once
#include<list>
#include"Question.h"
#include<fstream>
#include<iostream>

class QuestionManager
{
public:
	QuestionManager()=default;
	QuestionManager(std::ifstream& in);

public:
	const Question& GetNumericalQuestion();
	const Question& GetGridQuestion();

public:
	void ReadFile(std::ifstream& in);


private:
	std::list<Question> m_numericalQuestions;
	std::list<Question> m_gridQuestions;
};

