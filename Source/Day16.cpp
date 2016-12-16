#include "Days.h"

template <>
void Run<Day16>(Part part, istream& is, ostream& os)
{
    size_t MAX_CHARS = (part == Part01 ? 272 : 35651584);
    const string DATA = string(istreambuf_iterator<char>(is), {});

    // Ensure buffer is large enough
    string dragonData;
    dragonData.reserve(MAX_CHARS * 2);

    // Parse dragon data
    dragonData = DATA;
    while(dragonData.size() < MAX_CHARS)
    {
        // Middle 0
        dragonData.push_back('0');

        // Append string in reverse (skipping middle 0)
        auto it = dragonData.rbegin() + 1;
        while(it != dragonData.rend())
        {
            // Flip reversed chars (0 <-> 1)
            char c = char(*it ^ 1);
            dragonData.push_back(c);
            ++it;
        }
    }
    // Clamp size to max
    dragonData.resize(MAX_CHARS);

    // Determine checksum, writing directly into the buffer and then resizing to remove irrelevant chars
    string checkSum = dragonData;
    while(checkSum.size() > 0 && checkSum.size() % 2 == 0)
    {
        size_t j = 0;
        for(size_t i = 0; i < checkSum.size() - 1; i += 2)
        {
            checkSum[j] = (checkSum[i] == checkSum[i + 1] ? '1' : '0');
            ++j;
        }
        checkSum.resize(checkSum.size() / 2);
    }

    os << "Checksum for " << DATA << " with " << MAX_CHARS << " max characters:" << endl << checkSum << endl;
}