#include "Days.h"
#include "Point2D.h"
#include <array>
#include <bitset>
#include <map>
#include <queue>

// Find x*x + 3*x + 2*x*y + y + y*y
// Add the office designer's favorite number (your puzzle input).
// Find the binary representation of that sum; count the number of bits that are 1.
// If the number of bits that are 1 is even, it's an open space.
// If the number of bits that are 1 is odd, it's a wall.

static int magicNum = 0;

bool IsOpen(const Point2D& p)
{
    int x = p.x;
    int y = p.y;
    if(x >= 0 && y >= 0)
    {
        const int value = (x*x + 3 * x + 2 * x*y + y + y*y) + magicNum;
        const bitset<16> valueBits = bitset<16>(value);
        return (valueBits.count() % 2 == 0);
    }
    return false;
}

template <>
void Run<Day13>(Part part, istream& is, ostream& os)
{
    magicNum = stoi(string(istreambuf_iterator<char>(is), {}));

    const size_t BIG = size_t(1e9);
    const array<Point2D, 4> DIRS{ Point2D(-1, 0),Point2D(1, 0),Point2D(0, -1),Point2D(0, 1) };
    const Point2D P_START = Point2D(1, 1);
    const Point2D P_GOAL = (part == Part01 ? Point2D(31, 39) : Point2D(BIG, BIG));
    const size_t SEARCH_MAX = size_t(part == Part02 ? 50 : BIG);

    // Queue of points to visit
    queue<Point2D> pointsQueue = queue<Point2D>();
    pointsQueue.push(P_START);
    
    // Map of points visited and the distance from start to that point
    // A more complicated version of this would be to map points to their possible paths instead of a count
    map<Point2D, size_t> pointPathMap = map<Point2D, size_t>();
    pointPathMap.emplace(P_START, 0);

    while(!pointsQueue.empty() && pointPathMap.find(P_GOAL) == pointPathMap.end())
    {
        const Point2D& pointCur = pointsQueue.front();
        for(const Point2D& dir : DIRS)
        {
            const Point2D pointNext = (pointCur + dir);
            if(IsOpen(pointNext) && pointPathMap.find(pointNext) == pointPathMap.end())
            {
                const size_t searchDist = pointPathMap[pointCur];
                if(searchDist < SEARCH_MAX)
                {
                    pointsQueue.push(pointNext);
                    pointPathMap.emplace(pointNext, searchDist + 1);
                }
            }
        }
        pointsQueue.pop();
    }

    switch(part)
    {
        case Part01:
            os << "Path from (" << P_START.x << "," << P_START.y << ") to (" << P_GOAL.x << "," << P_GOAL.y << ")" << endl;
            if(pointPathMap.find(P_GOAL) == pointPathMap.end() )
            {
                os << "Failed to find path" << endl;
            }
            else
            {
                os << pointPathMap[P_GOAL] << " steps" << endl;
            }
            break;
        case Part02:
            os << "Max unique nodes visited within " << SEARCH_MAX << " steps: " << pointPathMap.size() << endl;
            break;
    }
}