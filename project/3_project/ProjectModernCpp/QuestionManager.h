#pragma once
#include<list>
#include"Question.h"
#include<fstream>

class QuestionManager
{
public:
	QuestionManager();
	QuestionManager(std::ifstream& in);

public:
	void ReadFile(std::ifstream& in);


private:
	std::list<Question> m_numericalQuestions;
	std::list<Question> m_gridQuestions;
};

