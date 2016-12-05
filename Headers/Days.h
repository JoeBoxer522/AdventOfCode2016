#ifndef DAYS_H
#define DAYS_H

using namespace std;

class Day
{
public:
	virtual void RunPart1() = 0;
	virtual void RunPart2() = 0;
};

class Day01 : public Day
{
public:
    virtual void RunPart1();
    virtual void RunPart2();
};

class Day02 : public Day
{
public:
    virtual void RunPart1();
    virtual void RunPart2();
};

#endif // DAYS_H