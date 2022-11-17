#include <iostream>
#include "QuestionManager.h"
#include <ctime>
int main()
{
	srand(time(0));
	std::ifstream in("Questions.txt");
	QuestionManager qm(in);
	
	Question q1(qm.GetGridQuestion());
	std::cout << q1;
	return 0;
}