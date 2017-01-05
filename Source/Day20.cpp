#include "Days.h"
#include <regex>
#include <vector>

bool IsWithin(size_t val, size_t min, size_t max)
{
    return(val >= min && val <= max);
}

template <>
void Run<Day20>(Part part, istream& is, ostream& os)
{
    const regex RX_RANGE(R"((\d+)-(\d+))");
    const size_t IP_MAX = 4294967295;
    vector<pair<size_t, size_t>> blackRanges;
    vector<size_t> whitelist;

    string arg;
    while(getline(is, arg))
    {
        smatch match;
        if(regex_match(arg, match, RX_RANGE))
        {
            const size_t rangeMin = size_t(stoul(match.str(1)));
            const size_t rangeMax = size_t(stoul(match.str(2)));
            blackRanges.push_back(make_pair(rangeMin, rangeMax));
        }
    }

    size_t ip = 0;
    size_t i = 0;
    while(ip < IP_MAX && (whitelist.size() == 0 || part == Part02))
    {
        for(i = 0; i < blackRanges.size(); ++i)
        {
            const auto& range = blackRanges[i];
            if(IsWithin(ip, range.first, range.second))
            {
                ip = range.second + 1;
                break;
            }
        }
        // Got through all ranges without a failure, this ip is valid
        if(i == blackRanges.size())
        {
            whitelist.push_back(ip);
            ++ip;
        }
    }

    if(part == Part01)
    {
        os << "Smallest possible IP: " << whitelist.front() << endl;
    }
    else if(part == Part02)
    {
        os << "Total possible IPs: " << whitelist.size() << endl;
    }
}