#include "Days.h"
#include <bitset>
#include <deque>

template <>
void Run<Day19>(Part part, istream& is, ostream& os)
{
    // Josephus Problem
    // www.youtube.com/watch?v=uCsD3ZGzMgE

    const int numElves = stoi(string(istreambuf_iterator<char>(is), {}));
    int winningElf = 0;
    
    if(part == Part01)
    {
        bitset<64> valueBits = bitset<64>(numElves);
        for(int i = int(valueBits.size()) - 1; i >= 0; --i)
        {
            // Find biggest 2^x within numElves (largest flipped bit in numElves)
            if(valueBits.test(i))
            {
                // l == remainder after removing this value
                int l = (numElves - int(pow(2, i)));
                winningElf = (2 * l) + 1;

                // Alternatively, we can move the largest bit to the end and parse the number
                for(int j = i; j > 0; --j)
                {
                    valueBits[j] = valueBits[j - 1];
                }
                valueBits.set(0, true);
                winningElf = int(valueBits.to_ulong());
                break;
            }
        }
    }
    else if(part == Part02)
    {
        deque<int> left;
        deque<int> right;
        for(int i = 1; i<=numElves; ++i)
        {
            if(i < numElves/2 + 1)
            {
                left.push_back(i);
            }
            else
            {
                right.push_front(i);
            }
        }

        while(left.size() && right.size())
        {
            if(left.size() > right.size())
            {
                left.pop_back();
            }
            else
            {
                right.pop_back();
            }

            right.push_front(left.front());
            left.pop_front();
            left.push_back(right.back());
            right.pop_back();
        }
        winningElf = (left.size() ? left.front() : right.front());
    }

    os << "Winning Elf: " << winningElf << endl;
}