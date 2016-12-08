#include "Days.h"

#include <algorithm>
#include <vector>

// SampleInput (part 1 output: easter, part 2 output: advent)
// eedadn
// drvtee
// eandsr
// raavrd
// atevrs
// tsrnev
// sdttsa
// rasrtv
// nssdts
// ntnada
// svetve
// tesnvt
// vntsnd
// vrdear
// dvrsen
// enarar

const size_t ALPHA_OFFSET = size_t('a');
const size_t MAX_ALPHA = (size_t('z') - ALPHA_OFFSET) + 1;

vector<vector<int>> frequentChars = vector<vector<int>>();

enum SORT_TYPE
{
    COUNT_MOST,
    COUNT_LEAST
};

string GetCorrectedMessage(SORT_TYPE sortType, istream& is)
{
    // Populate frequent characters by input column
    if(frequentChars.size() == 0)
    {
        string arg;
        while(getline(is, arg))
        {
            for(size_t i = 0; i < arg.length(); ++i)
            {
                if(frequentChars.size() <= i)
                {
                    frequentChars.push_back(vector<int>(MAX_ALPHA));
                }
                char c = arg.at(i);
                int cIdx = int(c) - ALPHA_OFFSET;
                frequentChars[i][cIdx]++;
            }
        }
    }

    string correctedMessage = string();
    for(size_t i = 0; i < frequentChars.size(); ++i)
    {
        char bestChar = '0';
        int bestCount = 0;
        switch(sortType)
        {
            case COUNT_MOST:
                bestCount = 0;
                break;
            case COUNT_LEAST:
                bestCount = int(1e9);
                break;
        }
        
        for(size_t j = 0; j < frequentChars[i].size(); ++j)
        {
            if((sortType == COUNT_MOST && frequentChars[i][j] > bestCount) ||
                (sortType == COUNT_LEAST && frequentChars[i][j] < bestCount))
            {
                bestChar = char(j + ALPHA_OFFSET);
                bestCount = frequentChars[i][j];
            }
        }
        correctedMessage.push_back(bestChar);
    }

    return correctedMessage;
}

template <>
void Run<Day06>(Part part, istream& is, std::ostream& os)
{
    SORT_TYPE sortType = part == Part01 ? COUNT_MOST : COUNT_LEAST;
    os << "Corrected message is: " << GetCorrectedMessage(sortType, is) << endl;
}