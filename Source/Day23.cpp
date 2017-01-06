#include "AdventMath.h"
#include "Days.h"
#include <regex>
#include <map>

static const regex RX_CPY(R"(cpy ([a-z]|-?\d+) ([a-z]))");
static const regex RX_INC(R"(inc ([a-z]))");
static const regex RX_DEC(R"(dec ([a-z]))");
static const regex RX_JNZ(R"(jnz ([a-z]|-?\d+) ([a-z]|-?\d+))");
static const regex RX_TGL(R"(tgl ([a-z]))");

enum eInsType
{
    I_NUL,
    I_CPY,
    I_INC,
    I_DEC,
    I_JNZ,
    I_TGL
};

struct Instruction
{
    eInsType type;
    string p1;
    string p2;

    Instruction() :
        type(I_NUL), p1(""), p2("")
    {}
    Instruction(eInsType inType, string inP1) :
        type(inType), p1(inP1), p2("")
    {}
    Instruction(eInsType inType, string inP1, string inP2) :
        type(inType), p1(inP1), p2(inP2)
    {}
};

bool GetValue(string str, const map<char, int>& dataMap, int& value)
{
    char c = str[0];
    if(isdigit(c) || c == '-')
    {
        value = stoi(str);
        return true;
    }
    else if(dataMap.find(c) != dataMap.end())
    {
        value = dataMap.at(c);
        return true;
    }
    
    return false;
}

template <>
void Run<Day23>(Part part, istream& is, ostream& os)
{
    vector<Instruction> instructions = vector<Instruction>();
    string arg;
    while(getline(is, arg))
    {
        Instruction ins = Instruction();

        smatch match;
        if(regex_match(arg, match, RX_CPY))
        {
            ins.type = I_CPY;
            ins.p1 = match.str(1);
            ins.p2 = match.str(2);
        }
        else if(regex_match(arg, match, RX_INC))
        {
            ins.type = I_INC;
            ins.p1 = match.str(1);
        }
        else if(regex_match(arg, match, RX_DEC))
        {
            ins.type = I_DEC;
            ins.p1 = match.str(1);
        }
        else if(regex_match(arg, match, RX_JNZ))
        {
            ins.type = I_JNZ;
            ins.p1 = match.str(1);
            ins.p2 = match.str(2);
        }
        else if(regex_match(arg, match, RX_TGL))
        {
            ins.type = I_TGL;
            ins.p1 = match.str(1);
        }
        instructions.push_back(ins);
    }

    map<char, int> dataMap = map<char, int>();
    const char charTest = 'a';
    dataMap[charTest] = (part == Part01 ? 7 : 12);

    for(size_t i = 0; i < instructions.size(); ++i)
    {
        const Instruction& ins = instructions[i];

        switch(ins.type)
        {
            case I_CPY:
                int cpyVal;
                if(GetValue(ins.p1, dataMap, cpyVal))
                {
                    char c = ins.p2[0];
                    dataMap[c] = cpyVal;
                }
                break;
            case I_INC:
                dataMap[ins.p1[0]]++;
                break;
            case I_DEC:
                dataMap[ins.p1[0]]--;
                break;
            case I_JNZ:
                int jnzVal;
                if(GetValue(ins.p1, dataMap, jnzVal) && jnzVal != 0)
                {
                    int jnzSkip;
                    if(GetValue(ins.p2, dataMap, jnzSkip))
                    {
                        i = Clamp(i + (jnzSkip - 1), size_t(0), instructions.size() - 1);
                    }
                }
                break;
            case I_TGL:
                size_t tglIdx = dataMap[ins.p1[0]] + i;
                if(tglIdx < instructions.size())
                {
                    Instruction& tglIns = instructions[tglIdx];
                    switch(tglIns.type)
                    {
                        case I_CPY:
                            tglIns.type = I_JNZ;
                            break;
                        case I_INC:
                            tglIns.type = I_DEC;
                            break;
                        case I_DEC:
                        case I_TGL:
                            tglIns.type = I_INC;
                            break;
                        case I_JNZ:
                            tglIns.type = I_CPY;
                            break;
                    }
                }
                break;
        }
    }

    os << "Value of " << charTest << " is " << dataMap[charTest] << endl;
}