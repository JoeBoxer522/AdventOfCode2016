// AdventOfCode2016.cpp
//

#include "Days.h"

int main(int argc, const char* argv[])
{
	Day* day = new Day02();
	day->RunPart1(argc, argv);
	day->RunPart2(argc, argv);
	delete day;

	return 0;
}

