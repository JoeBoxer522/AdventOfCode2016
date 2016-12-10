#include "Days.h"
#include <regex>

// Repeat Token is (#x#)
const static regex RX_REPEAT(R"(\((\d+)x(\d+)\))", regex::optimize);

size_t Decompress(const string& arg, bool decompressDeep)
{
    smatch match;
    if(!regex_search(arg, match, RX_REPEAT))
    {
        return arg.length();
    }
    size_t len = 0;

    // Parse the range of the chars to repeat and the loop count
    size_t repeatCharLen = stoi(match.str(1));
    size_t repeatCharLoops = stoi(match.str(2));

    // Decompress the substring if requested
    size_t matchIdx = match.position() + match.length();
    if(decompressDeep)
    {
        const string subStrDeep = arg.substr(matchIdx, repeatCharLen);
        len += Decompress(subStrDeep, decompressDeep) * repeatCharLoops;
    }
    else
    {
        len += repeatCharLen * repeatCharLoops;
    }
    const string subStr = arg.substr(matchIdx + repeatCharLen);
    len += Decompress(subStr, decompressDeep);
    return len;
}

template <>
void Run<Day09>(Part part, istream& is, ostream& os)
{
    string arg(istreambuf_iterator<char>(is), {});
    size_t dcCount = Decompress(arg, part == Part02);
    os << "Compressed string length: " << arg.length() << endl;
    os << "Decompressed string length: " << dcCount << endl;
}