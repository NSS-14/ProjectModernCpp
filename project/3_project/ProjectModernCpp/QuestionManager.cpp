#include "QuestionManager.h"

QuestionManager::QuestionManager()
{
}

QuestionManager::QuestionManager(std::ifstream& in)
{
	ReadFile(in);
}
