#include "Days.h"
#include <regex>

// Repeat Token is (#x#)
const static regex RX_REPEAT(R"(\((\d+)x(\d+)\))", regex::optimize);

size_t Decompress(string arg, bool decompressDeep)
{
    smatch match;
    if(!regex_search(arg, match, RX_REPEAT))
    {
        return arg.length();
    }
    size_t len = 0;

    // Parse the range of the chars to repeat and the loop count
    size_t charRange = stoi(match.str(1));
    size_t repeatCount = stoi(match.str(2));

    // Decompress the substring if requested
    size_t substrIdx = match.position() + match.length();
    if(decompressDeep)
    {
        const string subStr = arg.substr(substrIdx, charRange);
        len += Decompress(subStr, decompressDeep) * repeatCount;
    }
    else
    {
        len += charRange * repeatCount;
    }
    const string subStr = arg.substr(substrIdx + charRange);
    len += Decompress(subStr, decompressDeep);
    return len;
}

template <>
void Run<Day09>(Part part, istream& is, std::ostream& os)
{
    std::string arg(std::istreambuf_iterator<char>(is), {});
    size_t dcCount = Decompress(arg, part == Part02);
    os << "Compressed string length: " << arg.length() << endl;
    os << "Decompressed string length: " << dcCount << endl;
}