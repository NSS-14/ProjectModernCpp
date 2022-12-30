#pragma once
#include <vector>
#include "Question.h"
#include <fstream>
#include "StorageManager.h"

class QuestionManager
{
public:
	QuestionManager();
	QuestionManager(const QuestionManager& questionManager);
	QuestionManager(QuestionManager&& questionManager) noexcept;
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
	std::vector<Question> m_numericalQuestions;
	std::vector<Question> m_gridQuestions;
	size_t m_nextNumericalQuestionIndex;
	size_t m_nextGridQuestionIndex;
};

