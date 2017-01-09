#include "AdventMath.h"
#include "Days.h"
#include <regex>
#include <map>

static const regex RX_NOP(R"(nop)");
static const regex RX_CPY(R"(cpy ([a-z]|-?\d+) ([a-z]))");
static const regex RX_INC(R"(inc ([a-z]))");
static const regex RX_DEC(R"(dec ([a-z]))");
static const regex RX_ADD(R"(add ([a-z]) ([a-z]|-?\d+))");
static const regex RX_SUB(R"(sub ([a-z]) ([a-z]|-?\d+))");
static const regex RX_MUL(R"(mul ([a-z]) ([a-z]|-?\d+))");
static const regex RX_DIV(R"(div ([a-z]) ([a-z]|-?\d+))");
static const regex RX_JNZ(R"(jnz ([a-z]|-?\d+) ([a-z]|-?\d+))");
static const regex RX_TGL(R"(tgl ([a-z]))");

int GetValue(string str, const map<char, int>& dataMap)
{
    char c = str[0];
    if(isdigit(c) || c == '-')
    {
        return stoi(str);
    }
    else if(dataMap.find(c) != dataMap.end())
    {
        return dataMap.at(c);
    }
    return 0;
}

enum eInsType
{
    I_NOP,
    I_CPY,
    I_INC,
    I_DEC,
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_JNZ,
    I_TGL
};

struct Instruction
{
    eInsType type;
    string p1;
    string p2;

    Instruction() :
        type(I_NOP), p1(""), p2("")
    {}
    Instruction(eInsType inType, string inP1) :
        type(inType), p1(inP1), p2("")
    {}
    Instruction(eInsType inType, string inP1, string inP2) :
        type(inType), p1(inP1), p2(inP2)
    {}
};

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
        }
        else if(regex_match(arg, match, RX_INC))
        {
            ins.type = I_INC;
        }
        else if(regex_match(arg, match, RX_DEC))
        {
            ins.type = I_DEC;
        }
        else if(regex_match(arg, match, RX_ADD))
        {
            ins.type = I_ADD;
        }
        else if(regex_match(arg, match, RX_SUB))
        {
            ins.type = I_SUB;
        }
        else if(regex_match(arg, match, RX_MUL))
        {
            ins.type = I_MUL;
        }
        else if(regex_match(arg, match, RX_DIV))
        {
            ins.type = I_DIV;
        }
        else if(regex_match(arg, match, RX_JNZ))
        {
            ins.type = I_JNZ;
        }
        else if(regex_match(arg, match, RX_TGL))
        {
            ins.type = I_TGL;
        }
        ins.p1 = match.str(1);
        ins.p2 = match.str(2);
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
                dataMap[ins.p2[0]] = GetValue(ins.p1, dataMap);
                break;
            case I_INC:
                dataMap[ins.p1[0]]++;
                break;
            case I_DEC:
                dataMap[ins.p1[0]]--;
                break;
            case I_ADD:
                dataMap[ins.p2[0]] += GetValue(ins.p1, dataMap);
                break;
            case I_SUB:
                dataMap[ins.p2[0]] -= GetValue(ins.p1, dataMap);
                break;
            case I_MUL:
                dataMap[ins.p2[0]] *= GetValue(ins.p1, dataMap);
                break;
            case I_DIV:
                dataMap[ins.p2[0]] /= GetValue(ins.p1, dataMap);
                break;
            case I_JNZ:
                if(GetValue(ins.p1, dataMap) != 0)
                {
                    size_t jnzSkip = size_t(GetValue(ins.p2, dataMap));
                    i = Clamp(i + (jnzSkip), size_t(0), instructions.size() - 1) - 1;
                }
                break;
            case I_TGL:
                {
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
            case I_NOP:
            default:
                break;
        }
    }

    os << "Value of " << charTest << " is " << dataMap[charTest] << endl;
}