#include "AssemBunny.h"
#include "Days.h"

template <>
void Run<Day12>(Part part, istream& is, ostream& os)
{
    const char REG_TEST = 'a';
    map<char, int> dataMap = map<char, int>();
    if(part == Part02)
    {
        dataMap['c'] = 1;
    }

    vector<Instruction> instructions = Instruction::GetInstructions(is);
    for(size_t i = 0; i < instructions.size(); ++i)
    {
        const Instruction& ins = instructions[i];
        ins.Parse(dataMap, instructions, i);
    }

    os << "Value of " << REG_TEST << " is " << dataMap[REG_TEST] << endl;
}