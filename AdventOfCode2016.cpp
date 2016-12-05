// AdventOfCode2016.cpp
//

#include "stdafx.h"
#include "Days.h"

int main(int argc, const char* argv[])
{
	Day* day01 = new Day01();
	day01->RunPart1(argc, argv);
	day01->RunPart2(argc, argv);
	delete day01;

	return 0;
}

