#include "Days.h"
#include "MD5.h"
#include <regex>
#include <vector>

static string SALT("");
static size_t STRETCH_MAX(0);

bool FindKey(regex rx, smatch& match, size_t& idxCur, size_t idxMax, vector<string>& hashes, MD5& md5)
{
    while(idxCur <= idxMax)
    {
        if(hashes.size() <= idxCur)
        {
            string hash = md5.digestString(SALT + to_string(idxCur));
            for(size_t i = 0; i < STRETCH_MAX; ++i)
            {
                hash = md5.digestString(hash);
            }
            hashes.push_back(hash);
        }

        if(regex_search(hashes[idxCur], match, rx))
        {
            return true;
        }
        ++idxCur;
    }
    return false;
}

template <>
void Run<Day14>(Part part, istream& is, ostream& os)
{
    SALT = string(istreambuf_iterator<char>(is), {});
    STRETCH_MAX = part == Part02 ? 2016 : 0;
    
    MD5 md5 = MD5();
    const string STR_5XCHAR = R"((.)\1{4})";
    const regex RX_3XCHAR(R"((.)\1{2})");
    regex RX_5XCHAR(STR_5XCHAR);
    vector<string> hashes = vector<string>();

    const size_t SEARCH_MAX = 1000;
    size_t idxCur = 0;
    size_t idxMax = SEARCH_MAX;

    const int keysRequired = 64;
    int keysFound = 0;
    while(keysFound < keysRequired)
    {
        smatch match;
        if(FindKey(RX_3XCHAR, match, idxCur, idxMax, hashes, md5))
        {
            // 3x char key found, search for the next key w/ 5x of char
            char seekChar = match.str(1)[0];
            string str_rx = STR_5XCHAR;
            str_rx.replace(str_rx.find('.'), 1, 1, seekChar);
            RX_5XCHAR = regex(str_rx);

            ++idxCur;
            idxMax = idxCur + (SEARCH_MAX-1);
            size_t idxNext = idxCur;
            if(FindKey(RX_5XCHAR, match, idxCur, idxMax, hashes, md5))
            {
                ++keysFound;
            }

            // 2nd search over, resume search on next idx
            idxCur = idxNext;
        }
    }

    os << "Found " << keysFound << " keys at idx " << idxCur-1 << endl; 
}