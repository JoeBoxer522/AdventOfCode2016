#include "Days.h"
#include <regex>
#include <valarray>

const size_t COL_MAX = 50;
const size_t ROW_MAX = 6;

template <>
void Run<Day08>(Part part, istream& is, ostream& os)
{
    if(part == Part01)
    {
        const string CMD_RECT = "rect";
        const string CMD_ROTATE = "rotate";
        const string CMD_ROTATE_COLS = "x=";
        const string CMD_ROTATE_ROWS = "y=";
        const string CMD_ROTATE_AMT = "by ";

        // Initialize pixel grid (all off)
        char DisplayGrid[COL_MAX][ROW_MAX];
        for(int y = 0; y < ROW_MAX; ++y)
        {
            for(int x = 0; x < COL_MAX; ++x)
            {
                DisplayGrid[x][y] = '.';
            }
        }

        string arg;
        while(getline(is, arg))
        {
            if(arg.find(CMD_RECT) != string::npos)
            {
                size_t idxColumns = (CMD_RECT.length() + 1);
                size_t idxRows = arg.find('x');
                int numColumns = stoi(arg.substr(idxColumns, idxRows - idxColumns));
                int numRows = stoi(arg.substr(idxRows + 1));
                for(int x = 0; x < numColumns; ++x)
                {
                    for(int y = 0; y < numRows; ++y)
                    {
                        DisplayGrid[x][y] = '#';
                    }
                }
            }
            else if(arg.find(CMD_ROTATE) != string::npos)
            {
                size_t idxAmount = arg.find(CMD_ROTATE_AMT);
                size_t idxColumns = arg.find(CMD_ROTATE_COLS);
                size_t idxRows = arg.find(CMD_ROTATE_ROWS);

                int amount = 0;
                if(idxAmount != string::npos)
                {
                    int col = 0;
                    int row = 0;

                    amount = stoi(arg.substr(idxAmount + CMD_ROTATE_AMT.length()));
                    if(idxColumns != string::npos)
                    {
                        // Adjust idx to reach actual number
                        idxColumns += CMD_ROTATE_COLS.length();
                        col = stoi(arg.substr(idxColumns, idxAmount - idxColumns));

                        for(int i = 0; i < amount; ++i)
                        {
                            row = 0;
                            char charCur = DisplayGrid[col][row];
                            char charPrev = DisplayGrid[col][ROW_MAX - 1];
                            while(row < ROW_MAX)
                            {
                                charCur = DisplayGrid[col][row];
                                DisplayGrid[col][row] = charPrev;
                                charPrev = charCur;
                                ++row;
                            }
                        }
                    }
                    else if(idxRows != string::npos)
                    {
                        // Adjust idx to reach actual number
                        idxRows += CMD_ROTATE_ROWS.length();
                        row = stoi(arg.substr(idxRows, idxAmount - idxRows));

                        for(int i = 0; i < amount; ++i)
                        {
                            col = 0;
                            char charCur = DisplayGrid[col][row];
                            char charPrev = DisplayGrid[COL_MAX - 1][row];
                            while(col < COL_MAX)
                            {
                                charCur = DisplayGrid[col][row];
                                DisplayGrid[col][row] = charPrev;
                                charPrev = charCur;
                                ++col;
                            }
                        }
                    }
                }
            }
            // Print grid at each update
    //         os << string(1000, '\n');
    //         for(int y = 0; y < ROW_MAX; ++y)
    //         {
    //             for(int x = 0; x < COL_MAX; ++x)
    //             {
    //                 os << DisplayGrid[x][y];
    //             }
    //             os << endl;
    //         }
        }

        int numPixelsLit = 0;
        for(int y = 0; y < ROW_MAX; ++y)
        {
            for(int x = 0; x < COL_MAX; ++x)
            {
                if(DisplayGrid[x][y] == '#')
                {
                    ++numPixelsLit;
                }
            }
        }

        os << "Number of Pixels Lit: " << numPixelsLit << endl;
    }
    else if(part == Part02)
    {
        // Alternate solution
        const regex RX_RECT{ R"(rect (\d+)x(\d+))" };
        const regex RX_ROT_COL{ R"(rotate column x=(\d+) by (\d+))" };
        const regex RX_ROT_ROW{ R"(rotate row y=(\d+) by (\d+))" };
        valarray<bool> displayGrid(COL_MAX * ROW_MAX);

        string arg;
        while(getline(is, arg))
        {
            smatch match;
            if(regex_match(arg, match, RX_RECT))
            {
                unsigned int numCols = stoi(match.str(1));
                unsigned int numRows = stoi(match.str(2));

                // Create slice in the display grid for the new rectangle
                gslice sliceRect = gslice{ 0, { numRows, numCols }, { COL_MAX, 1 } }; // starting index, set of lengths, set of strides
                displayGrid[sliceRect] = true;
            }
            else
            {
                unsigned int col, row, dist;
                if(regex_match(arg, match, RX_ROT_COL))
                {
                    col = stoi(match.str(1));
                    dist = stoi(match.str(2)) % ROW_MAX;

                    // Get column 'slice' and convert to a val array
                    // Rotate desired distance
                    // Replace grid column with rotated slice
                    slice sliceColumn = slice{ col, ROW_MAX, COL_MAX }; // starting index, length, stride
                    valarray<bool> arrayColumn{ displayGrid[sliceColumn] };
                    rotate(begin(arrayColumn), end(arrayColumn) - dist, end(arrayColumn));
                    displayGrid[sliceColumn] = arrayColumn;
                }
                else if(regex_match(arg, match, RX_ROT_ROW))
                {
                    row = stoi(match.str(1));
                    dist = stoi(match.str(2)) % COL_MAX;

                    // Get row 'slice' and convert to a val array
                    // Rotate desired distance
                    // Replace grid row with rotated slice
                    slice sliceRow = slice{ COL_MAX * row, COL_MAX, 1 }; // starting index, length, stride
                    valarray<bool> arrayRow{ displayGrid[sliceRow] };
                    rotate(begin(arrayRow), end(arrayRow) - dist, end(arrayRow));
                    displayGrid[sliceRow] = arrayRow;
                }
            }
        }

        for(int y = 0; y < ROW_MAX; ++y)
        {
            for(int x = 0; x < COL_MAX; ++x)
            {
                os << (displayGrid[y * COL_MAX + x] ? '#' : '.');
            }
            os << endl;
        }

        //os << "Number of Pixels Lit: " << count(begin(displayGrid), end(displayGrid), true) << endl;
    }
}