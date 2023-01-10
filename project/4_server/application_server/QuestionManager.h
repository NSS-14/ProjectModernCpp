#pragma once
#include <vector>

#include "Question.h"
#include "StorageManager.h"

class QuestionManager
{
public:
	QuestionManager();
	QuestionManager(const QuestionManager& questionManager);
	QuestionManager(QuestionManager&& questionManager) noexcept;

public:
	QuestionManager& operator =(const QuestionManager& questionManager);
	QuestionManager& operator =(QuestionManager&& questionManager) noexcept;

public:
	const Question& GetNumericalQuestion() const;
	const Question& GetGridQuestion() const;

public:
	void ReadDataBase(Storage& db);

private:
	std::vector<Question> m_numericalQuestions;
	std::vector<Question> m_gridQuestions;
	mutable size_t m_nextNumericalQuestionIndex;
	mutable size_t m_nextGridQuestionIndex;
};
