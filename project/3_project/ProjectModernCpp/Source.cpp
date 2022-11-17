#include <iostream>
#include "QuestionManager.h"

int main()
{
	std::ifstream in("Questions.txt");
	QuestionManager qm(in);
	
	return 0;
}