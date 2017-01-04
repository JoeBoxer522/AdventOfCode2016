#include "Days.h"
#include <vector>

// Rules to determine traps in next row based on current row
// Its left and center tiles are traps, but its right tile is not.
// Its center and right tiles are traps, but its left tile is not.
// Only its left tile is a trap.
// Only its right tile is a trap.

static const char CHAR_SAFE = '.';
static const char CHAR_TRAP = '^';

bool IsTrap(int idx, const string& s)
{
    if(idx >= 0 && idx < s.size())
    {
        return s[idx] == CHAR_TRAP;
    }
    return false;
}

template <>
void Run<Day18>(Part part, istream& is, ostream& os)
{   
    size_t safeTiles = 0;
    int numRows = ((part == Part01) ? 40 : 400000);

    // Use input for initial row
    string rowCur = string(istreambuf_iterator<char>(is), {});
    while(numRows > 0)
    {
        string rowNext = string();
        for(int i = 0; i<rowCur.size(); ++i)
        {
            if(rowCur[i] == CHAR_SAFE) { safeTiles++; }

            if
            (
                (IsTrap(i-1, rowCur) && IsTrap(i, rowCur) && !IsTrap(i+1, rowCur)) ||   // Rule 1
                (!IsTrap(i-1, rowCur) && IsTrap(i, rowCur) && IsTrap(i+1, rowCur)) ||   // Rule 2
                (IsTrap(i-1, rowCur) && !IsTrap(i, rowCur) && !IsTrap(i+1, rowCur)) ||  // Rule 3
                (!IsTrap(i-1, rowCur) && !IsTrap(i, rowCur) && IsTrap(i+1, rowCur))     // Rule 4
            )
            {
                rowNext.push_back(CHAR_TRAP);
            }
            else
            {
                rowNext.push_back(CHAR_SAFE);
            }
        }
        rowCur = rowNext;
        numRows--;
    }
    os << "Safe Tiles: " << safeTiles << endl;
}