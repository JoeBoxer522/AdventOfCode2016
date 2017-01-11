#include "AssemBunny.h"
#include "Days.h"

template <>
void Run<Day25>(Part part, istream& is, ostream& os)
{
    if(part == Part01)
    {
        const char REG_TEST = 'a';
        const int MAX_PATTERN = 100;
        map<char, int> dataMap = map<char, int>();

        // Loop until a sufficient pattern is found
        int patternCount = 0;
        int valTest = 0;
        while(patternCount < MAX_PATTERN)
        {
            int sigExpected = 0;
            dataMap[REG_TEST] = valTest;
            is.clear();
            is.seekg(0, ios::beg);
            vector<Instruction> instructions = Instruction::GetInstructions(is);
            for(size_t i = 0; i < instructions.size() && patternCount < MAX_PATTERN; ++i)
            {
                const Instruction& ins = instructions[i];
                ins.Parse(dataMap, instructions, i);
                if(ins.type == I_OUT)
                {
                    int sigOut = Instruction::GetValue(ins.p1, dataMap);
                    if(sigOut == sigExpected)
                    {
                        sigExpected = (sigExpected == 0) ? 1 : 0;
                        ++patternCount;
                    }
                    else
                    {
                        // Pattern failed, reset and restart instructions
                        patternCount = 0;
                        dataMap.clear();
                        instructions.clear();
                        ++valTest;
                        break;
                    }
                }
            }
        }

        os << "Best starting val for register " << REG_TEST << ": " << valTest << endl;
    }
}