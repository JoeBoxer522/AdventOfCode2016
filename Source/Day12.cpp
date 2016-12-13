#include "AdventMath.h"
#include "Days.h"
#include <regex>
#include <map>

static const regex RX_CPY(R"(cpy ([a-z]|\d+) ([a-z]))");
static const regex RX_INC(R"(inc ([a-z]))");
static const regex RX_DEC(R"(dec ([a-z]))");
static const regex RX_JNZ(R"(jnz ([a-z]|-?\d+) (-?\d+))");

template <>
void Run<Day12>(Part part, istream& is, ostream& os)
{
    vector<string> instructions = vector<string>();
    string arg;
    while(getline(is, arg))
    {
        instructions.push_back(arg);
    }

    map<char, int> dataMap = map<char, int>();
    const char charTest = 'a';

    if(part == Part02)
    {
        dataMap['c'] = 1;
    }

    for(size_t i = 0; i < instructions.size(); ++i)
    {
        const string& instruction = instructions[i];
        //os << instruction << endl;

        smatch match;
        if(regex_match(instruction, match, RX_CPY))
        {
            char c = match.str(2)[0];
            if(isdigit(match.str(1)[0]))
            {
                int value = stoi(match.str(1));
                dataMap[c] = value;

                //os << "Copy " << value << " to " << c << endl;
            }
            else
            {
                char addr = match.str(1)[0];
                dataMap[c] = dataMap[addr];

                //os << "Copy " << addr << "(" << dataMap[addr] << ") to " << c << endl;
            }
        }
        else if(regex_match(instruction, match, RX_INC))
        {
            char c = match.str(1)[0];
            dataMap[c]++;

            //os << "Increment " << c << " to " << dataMap[c] << endl;
        }
        else if(regex_match(instruction, match, RX_DEC))
        {
            char c = match.str(1)[0];
            dataMap[c]--;

            //os << "Decrement " << c << " to " << dataMap[c] << endl;
        }
        else if(regex_match(instruction, match, RX_JNZ))
        {
            int value = 0;
            int numSkip = 0;
            if(isdigit(match.str(1)[0]))
            {
                value = stoi(match.str(1));
                //os << "value is ";
            }
            else
            {
                char c = match.str(1)[0];
                value = dataMap[c];
                //os << c << " is ";
            }
            if(value != 0)
            {
                numSkip = stoi(match.str(2));
                i = Clamp(i + (numSkip - 1), size_t(0), instructions.size() - 1);
            }
            //os << value << ", skip " << numSkip << " instructions" << endl;
        }
    }

    os << "Value of " << charTest << " is " << dataMap[charTest] << endl;
}