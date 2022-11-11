#include <iostream>
#include"Map.h"

int main()
{
	Region region;
	Map map(6,4);
	map[{1, 1}] = region;
	std::cout << map;

	
	return 0;
}