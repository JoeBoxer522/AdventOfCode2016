#include "Days.h"
#include <regex>

// swap position X with position Y means that the letters at indexes X and Y(counting from 0) should be swapped.
// swap letter X with letter Y means that the letters X and Y should be swapped(regardless of where they appear in the string).
// rotate left / right X steps means that the whole string should be rotated; for example, one right rotation would turn abcd into dabc.
// rotate based on position of letter X means that the whole string should be rotated to the right based on the index of letter X(counting from 0) as determined before this instruction does any rotations.Once the index is determined, rotate the string to the right one time, plus a number of times equal to that index, plus one additional time if the index was at least 4.
// reverse positions X through Y means that the span of letters at indexes X through Y(including the letters at X and Y) should be reversed in order.
// move position X to position Y means that the letter which is at index X should be removed from the string, then inserted such that it ends up at index Y.

void swap(string& s, int i1, int i2)
{
    const int len = int(s.length());
    while(i1 >= len)
    {
        i1 = len - i1;
    }
    while(i2 >= len)
    {
        i2 = len - i2;
    }
    while(i1 < 0)
    {
        i1 += len;
    }
    while(i2 < 0)
    {
        i2 += len;
    }
    char tmp = s[i1];
    s[i1] = s[i2];
    s[i2] = tmp;
}

void rotate(string& s, bool left, int count)
{
    while(count > 0)
    {
        if(left)
        {
            for(int i = 0; i < static_cast<int>(s.length())-1; ++i)
            {
                swap(s, i, i+1);
            }
        }
        else
        {
            for(int i = static_cast<int>(s.length())-1; i > 0; --i)
            {
                swap(s, i, i+1);
            }
        }
        --count;
    }
}

template <>
void Run<Day21>(Part part, istream& is, ostream& os)
{
    const regex RX_SWAP_POS(R"(swap position (\d+) with position (\d+))");
    const regex RX_SWAP_LET(R"(swap letter ([a-z]) with letter ([a-z]))");
    const regex RX_ROT(R"(rotate (\w+) (\d+) step[s]?)");
    const regex RX_ROT_POS(R"(rotate based on position of letter ([a-z]))");
    const regex RX_REVERSE(R"(reverse positions (\d+) through (\d+))");
    const regex RX_MOVE(R"(move position (\d+) to position (\d+))");

    vector<string> instructions = vector<string>();
    string arg;
    while(getline(is, arg))
    {
        instructions.push_back(arg);
    }

    if(part == Part01)
    {
        string PASSWORD = "abcdefgh";
        string scrambled = PASSWORD;

        for(const string& instruction : instructions)
        {
            smatch match;
            if(regex_match(instruction, match, RX_SWAP_POS))
            {
                int i1 = stoi(match.str(1));
                int i2 = stoi(match.str(2));
                swap(scrambled, i1, i2);
            }
            else if(regex_match(instruction, match, RX_SWAP_LET))
            {
                char c1 = match.str(1).front();
                char c2 = match.str(2).front();
                int i1 = int(scrambled.find(c1));
                int i2 = int(scrambled.find(c2));
                swap(scrambled, i1, i2);
            }
            else if(regex_match(instruction, match, RX_ROT))
            {
                bool left = (match.str(1) == "left");
                int count = stoi(match.str(2));
                rotate(scrambled, left, count);
            }
            else if(regex_match(instruction, match, RX_ROT_POS))
            {
                char c = match.str(1).front();
                int idx = int(scrambled.find(c));
                int count = idx + (idx > 3 ? 2 : 1);
                rotate(scrambled, false, count);
            }
            else if(regex_match(instruction, match, RX_REVERSE))
            {
                int i1 = stoi(match.str(1));
                int i2 = stoi(match.str(2));
                for(int i = i1, j = i2; i < j; ++i, --j)
                {
                    swap(scrambled, i, j);
                }
            }
            else if(regex_match(instruction, match, RX_MOVE))
            {
                int i1 = stoi(match.str(1));
                int i2 = stoi(match.str(2));
                char c = scrambled[i1];
                scrambled.erase(scrambled.begin() + i1);
                scrambled.insert(scrambled.begin() + i2, c);
            }
        }

        os << "Password: " << PASSWORD << endl << "Scrambled: " << scrambled << endl;
    }
    else if(part == Part02)
    {
        const string SCRAMBLED = "fbgdceah";
        string password = SCRAMBLED;

        for(int iIdx = int(instructions.size())-1; iIdx >= 0; --iIdx)
        {
            const string& instruction = instructions[iIdx];

            smatch match;
            if(regex_match(instruction, match, RX_SWAP_POS))
            {
                int i1 = stoi(match.str(1));
                int i2 = stoi(match.str(2));
                swap(password, i2, i1);
            }
            else if(regex_match(instruction, match, RX_SWAP_LET))
            {
                char c1 = match.str(1).front();
                char c2 = match.str(2).front();
                int i1 = int(password.find(c1));
                int i2 = int(password.find(c2));
                swap(password, i2, i1);
            }
            else if(regex_match(instruction, match, RX_ROT))
            {
                bool left = (match.str(1) == "left");
                int count = stoi(match.str(2));
                rotate(password, !left, count); // Rotate opposite of instruction
            }
            else if(regex_match(instruction, match, RX_ROT_POS))
            {
                char c = match.str(1).front();
                int idx = int(password.find(c));
                int count = idx;
                if(idx != 0 && idx % 2 == 0)
                {
                    count += int(password.size());
                }
                count = ((count / 2) + 1) % int(password.size());
                rotate(password, true, count);
            }
            else if(regex_match(instruction, match, RX_REVERSE))
            {
                int i1 = stoi(match.str(1));
                int i2 = stoi(match.str(2));
                for(int i = i1, j = i2; i < j; ++i, --j)
                {
                    swap(password, i, j);
                }
            }
            else if(regex_match(instruction, match, RX_MOVE))
            {
                int i1 = stoi(match.str(2));
                int i2 = stoi(match.str(1));
                char c = password[i1];
                password.erase(password.begin() + i1);
                password.insert(password.begin() + i2, c);
            }
        }

        os << "Password: " << password << endl << "Scrambled: " << SCRAMBLED << endl;
    }
}