#include "Days.h"
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

struct Point
{
    int x;
    int y;

    Point() :
        x(0), y(0)
    {}
    Point(int inX, int inY) :
        x(inX), y(inY)
    {}

    bool IsOpen() const
    {
        if(x >= 0 && y >=0)
        {
            const int value = (x*x + 3*x + 2*x*y + y + y*y) + magicNum;
            const bitset<16> valueBits = bitset<16>(value);
            return (valueBits.count() % 2 == 0);
        }
        return false;
    }
    bool operator==(const Point& p) const
    {
        return(x == p.x && y == p.y);
    }
    bool operator!=(const Point& p) const
    {
        return(x != p.x || y != p.y);
    }
    bool operator<(const Point& p) const
    {
        return y == p.y ? x < p.x : y < p.y;
    }
    Point operator+(const Point& p) const
    {
        return(Point(x+p.x, y+p.y));
    }
};

template <>
void Run<Day13>(Part part, istream& is, ostream& os)
{
    magicNum = stoi(string(istreambuf_iterator<char>(is), {}));

    const size_t BIG = size_t(1e9);
    const array<Point, 4> DIRS{ Point(-1, 0),Point(1, 0),Point(0, -1),Point(0, 1) };
    const Point P_START = Point(1, 1);
    const Point P_GOAL = (part == Part01 ? Point(31, 39) : Point(BIG, BIG));
    const size_t SEARCH_MAX = size_t(part == Part02 ? 50 : BIG);

    // Queue of points to visit
    queue<Point> pointsQueue = queue<Point>();
    pointsQueue.push(P_START);
    
    // Map of points visited and the distance from start to that point
    // A more complicated version of this would be to map points to their possible paths instead of a count
    map<Point, size_t> pointPathMap = map<Point, size_t>();
    pointPathMap.emplace(P_START, 0);

    while(!pointsQueue.empty() && pointPathMap.find(P_GOAL) == pointPathMap.end())
    {
        const Point& pointCur = pointsQueue.front();
        for(const Point& dir : DIRS)
        {
            const Point pointNext = (pointCur + dir);
            if(pointNext.IsOpen() && pointPathMap.find(pointNext) == pointPathMap.end())
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