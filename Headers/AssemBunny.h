#ifndef ASSEMBUNNY_H
#define ASSEMBUNNY_H

#include "AdventMath.h"
#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <vector>

using namespace std;

enum eInsType
{
    I_NOP,
    I_INC,
    I_DEC,
    I_OUT,
    I_CPY,
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

    static int GetValue(string str, const map<char, int>& dataMap)
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

    static vector<Instruction> GetInstructions(istream& is)
    {
        static map<eInsType, regex> rxTypeMap;
        if(rxTypeMap.size() == 0)
        {
            rxTypeMap.emplace(I_NOP, R"(nop)");
            rxTypeMap.emplace(I_INC, R"(inc ([a-z]))");
            rxTypeMap.emplace(I_DEC, R"(dec ([a-z]))");
            rxTypeMap.emplace(I_OUT, R"(out ([a-z]|-?\d+))");
            rxTypeMap.emplace(I_CPY, R"(cpy ([a-z]|-?\d+) ([a-z]))");
            rxTypeMap.emplace(I_ADD, R"(add ([a-z]|-?\d+) ([a-z]))");
            rxTypeMap.emplace(I_SUB, R"(sub ([a-z]|-?\d+) ([a-z]))");
            rxTypeMap.emplace(I_MUL, R"(mul ([a-z]|-?\d+) ([a-z]))");
            rxTypeMap.emplace(I_DIV, R"(div ([a-z]|-?\d+) ([a-z]))");
            rxTypeMap.emplace(I_JNZ, R"(jnz ([a-z]|-?\d+) ([a-z]|-?\d+))");
            rxTypeMap.emplace(I_TGL, R"(tgl ([a-z]))");
        }

        vector<Instruction> instructions = vector<Instruction>();
        string arg;
        while(getline(is, arg))
        {
            Instruction ins = Instruction();
            smatch match;
            for(const auto& pair : rxTypeMap)
            {
                if(regex_match(arg, match, pair.second))
                {
                    ins.type = pair.first;
                    ins.p1 = match.str(1);
                    ins.p2 = match.str(2);
                    break;
                }
            }
            instructions.push_back(ins);
        }
        return instructions;
    }

    void Parse(map<char, int>& dataMap, vector<Instruction>& instructions, size_t& i) const
    {
        switch(type)
        {
            case I_INC:
                dataMap[p1[0]]++;
                break;
            case I_DEC:
                dataMap[p1[0]]--;
                break;
            case I_OUT:
                //GetValue(p1, dataMap);
                break;
            case I_CPY:
                dataMap[p2[0]] = GetValue(p1, dataMap);
                break;
            case I_ADD:
                dataMap[p2[0]] += GetValue(p1, dataMap);
                break;
            case I_SUB:
                dataMap[p2[0]] -= GetValue(p1, dataMap);
                break;
            case I_MUL:
                dataMap[p2[0]] *= GetValue(p1, dataMap);
                break;
            case I_DIV:
                dataMap[p2[0]] /= GetValue(p1, dataMap);
                break;
            case I_JNZ:
                if(GetValue(p1, dataMap) != 0)
                {
                    size_t jnzSkip = size_t(GetValue(p2, dataMap));
                    i = Clamp(i + (jnzSkip), size_t(0), instructions.size() - 1) - 1;
                }
                break;
            case I_TGL:
                {
                    size_t tglIdx = dataMap[p1[0]] + i;
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
};
#endif // ASSEMBUNNY_H