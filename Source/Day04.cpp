#include "Days.h"

#include <algorithm>
#include <math.h>
#include <vector>

static vector<string> validRooms = vector<string>();

struct CharCount
{
    char c;
    size_t count;

    CharCount() :
        c(' '), count(0)
    {}

    CharCount(char inC) :
        c(inC), count(1)
    {}

    CharCount(char inC, size_t inCount) :
        c(inC), count(inCount)
    {}

    bool operator<(const CharCount& charCount) const { return count > charCount.count || (count == charCount.count && toupper(c) < toupper(charCount.c)); }
};

string GetChecksum(string arg)
{
    size_t start = arg.find('[', 0) + 1;
    size_t end = arg.find(']', start);
    size_t count = (end - start);
    return(arg.substr(start, count));
}

int GetSector(string arg, ostream& os)
{
    size_t start = arg.find_last_of('-') + 1;
    size_t end = arg.find('[', start);
    size_t count = (end - start);
    string subStr = arg.substr(start, count);
    try
    {
        return(stoi(subStr));
    }
    catch(...)
    {
        os << "Error: Unable to convert \"" << subStr << "\" to int\n";
    }
    return(0);
}

void IncrementChar(vector<CharCount>& charCounts, char c)
{
    for(auto& charCount : charCounts)
    {
        if(charCount.c == c)
        {
            charCount.count++;
            return;
        }
    }
    charCounts.push_back(CharCount(c));
}

bool IsChecksumValid(string checkSum, const vector<CharCount>& charCounts)
{
    if(checkSum.length() <= charCounts.size())
    {
        for(size_t i = 0; i < checkSum.length(); ++i)
        {
            if(checkSum[i] != charCounts[i].c)
            {
                return(false);
            }
        }
        return(true);
    }
    return(false);
}

template <>
void Run<Day04>(Part part, istream& is, ostream& os)
{
    if(part == Part01)
    {
        // aaaaa - bbb - z - y - x - 123[abxyz]
        // a - b - c - d - e - f - g - h - 987[abcde]
        // not- a - real - room - 404[oarel]
        // totally - real - room - 200[decoy]

        int totalRooms = 0;
        int sumSectors = 0;
        vector<CharCount> charCounts = vector<CharCount>();
        validRooms.clear();

        string arg;
        while(getline(is, arg))
        {
            // Read encrypted letters
            size_t end = arg.find_last_of('-');
            for(size_t i = 0; i < end; ++i)
            {
                if(arg[i] != '-')
                {
                    IncrementChar(charCounts, arg[i]);
                }
            }

            // Sort first on count, then alphabetical (See CharCount '<' operator)
            sort(charCounts.begin(), charCounts.end());

            // Read checkSum and compare
            string checkSum = GetChecksum(arg);
            if(IsChecksumValid(checkSum, charCounts))
            {
                sumSectors += GetSector(arg, os);
                validRooms.push_back(arg);
            }
            charCounts.clear();
            ++totalRooms;
        }

        os << "Total Rooms: " << totalRooms << endl;
        os << "Valid Rooms: " << validRooms.size() << endl;
        os << "Sum of Sectors: " << sumSectors << endl;
    }
    else
    {
        const int intA = int('a');
        const int intZ = int('z');
        const int alphaDelta = (intZ - intA) + 1;

        os << "Decrypted \"north\" rooms: " << endl;
        for(const string& room : validRooms)
        {
            // Calculate relative alpha increment based on how many times the sector value 'loops' around alphabet 
            int sector = GetSector(room, os);
            int sectorIncrement = sector % alphaDelta;
            string s = string();

            // Read encrypted letters
            size_t end = room.find_last_of('-');
            for(size_t i = 0; i < end; ++i)
            {
                char c = room[i];
                if(c == '-')
                {
                    c = ' ';
                }
                else
                {
                    if(c + sectorIncrement > intZ)
                    {
                        // Wrap around alphabet using increment and offsets
                        c = (intA - 1) + (sectorIncrement - (intZ - c));
                    }
                    else
                    {
                        // No need to wrap, simply increment the char
                        c += sectorIncrement;
                    }
                }
                s.push_back(c);
            }
            if(s.substr(0, 5) == "north")
            {
                // Check north rooms to solve this puzzle
                os << "Room: " << s << "\nSector: " << sector << endl;
            }
        }
    }
}