#include <algorithm>
#include <vector>

#include "AdventMath.h"
#include "Days.h"

//1 2 3
//4 5 6
//7 8 9
static const int MAX_NUMPAD = 3;
static const char NUM_PAD[MAX_NUMPAD][MAX_NUMPAD] = { { '1','4','7' },{ '2','5','8' },{ '3','6','9' } };

//     1
//   2 3 4
// 5 6 7 8 9
//   A B C
//     D
static const int MAX_NUMPADWEIRD = 5;
static const char NUM_PAD_WEIRD[MAX_NUMPADWEIRD][MAX_NUMPADWEIRD] = { { ' ',' ','5',' ',' ' },{ ' ','2','6','A',' ' },{ '1','3','7','B','D' },{ ' ','4','8','C',' ' },{ ' ',' ','9',' ',' ' } };

bool TestNumPadWeird(int x, int y)
{
    if(x < 0 || x >= MAX_NUMPADWEIRD)
    {
        return(false);
    }
    if(y < 0 || y >= MAX_NUMPADWEIRD)
    {
        return(false);
    }
    if(NUM_PAD_WEIRD[x][y] == ' ')
    {
        return(false);
    }
    return(true);
}

template <>
void Run<Day02>(Part part, istream& is, ostream& os)
{
    if(part == Part01)
    {
        // ULL
        // RRDDD
        // LURDL
        // UUUUD

        // Start at 5
        int x = 1;
        int y = 1;

        os << "Bathroom code is: ";
        string arg;
        while(getline(is, arg))
        {
            for(size_t i = 0; i < arg.length(); ++i)
            {
                switch(arg[i])
                {
                    case 'U':
                        y = Clamp(y - 1, 0, MAX_NUMPAD - 1);
                        break;
                    case 'D':
                        y = Clamp(y + 1, 0, MAX_NUMPAD - 1);
                        break;
                    case 'L':
                        x = Clamp(x - 1, 0, MAX_NUMPAD - 1);
                        break;
                    case 'R':
                        x = Clamp(x + 1, 0, MAX_NUMPAD - 1);
                        break;
                }
            }
            os << NUM_PAD[x][y];
        }
        os << endl;
    }
    else if(part == Part02)
    {
        // ULL
        // RRDDD
        // LURDL
        // UUUUD

        // Start at 5
        int x = 0;
        int y = 2;

        os << "Bathroom code is: ";
        string arg;
        while(getline(is, arg))
        {
            for(size_t i = 0; i < arg.length(); ++i)
            {
                switch(arg[i])
                {
                    case 'U':
                        if(TestNumPadWeird(x, y - 1))
                        {
                            y -= 1;
                        }
                        break;
                    case 'D':
                        if(TestNumPadWeird(x, y + 1))
                        {
                            y += 1;
                        }
                        break;
                    case 'L':
                        if(TestNumPadWeird(x - 1, y))
                        {
                            x -= 1;
                        }
                        break;
                    case 'R':
                        if(TestNumPadWeird(x + 1, y))
                        {
                            x += 1;
                        }
                        break;
                }
            }
            os << NUM_PAD_WEIRD[x][y];
        }
        os << endl;
    }
}