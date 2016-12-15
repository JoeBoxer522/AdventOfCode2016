#include "Days.h"
#include <regex>
#include <vector>

struct Disc
{
    Disc() :
        posInit(0), positions(0)
    {}

    Disc(int init, int max) :
        posInit(init), positions(max)
    {}
    int GetPosAtTime(int t) const
    {
        return(positions > 0 ? ((posInit + t) % positions) : 0);
    }
private:
    int posInit;
    int positions;
};

// Check if the discs are aligned (at position 0) when button is pressed for timestamp 't'
bool AreDiscsAligned(vector<Disc>& discs, int t)
{
    for(int i = 0; i < discs.size(); ++i)
    {
        const Disc& d = discs[i];
        const int discNum = i + 1;
        if(d.GetPosAtTime(t + discNum) != 0)
        {
            return false;
        }
    }
    return(discs.size() > 0);
}

template <>
void Run<Day15>(Part part, istream& is, ostream& os)
{
    const regex RX_DISC(R"(Disc #(\d+) has (\d+) positions; at time=(\d+), it is at position (\d+).)");

    vector<Disc> discs;
    string arg;
    while(getline(is, arg))
    {
        smatch match;
        if(regex_match(arg, match, RX_DISC))
        {
            int discNum = stoi(match.str(1));
            if(discs.size() < discNum)
            {
                discs.resize(discNum);
            }
            
            Disc d = Disc(stoi(match.str(4)), stoi(match.str(2)));
            discs[discNum-1] = d;
        }
    }
    if(part == Part02)
    {
        discs.push_back(Disc(0,11));
    }

    int t = 0;
    while(!AreDiscsAligned(discs, t))
    {
        ++t;
    }

    os << discs.size() << " discs align at timestamp " << t << endl;
}