#include "Days.h"
#include "MD5.h"
#include "Point2D.h"

#include <array>
#include <queue>
#include <vector>

template <>
void Run<Day17>(Part part, istream& is, ostream& os)
{
    const string passcode = string(istreambuf_iterator<char>(is), {});

    const array<char, 5> OPEN_CHARS = { 'b', 'c', 'd', 'e', 'f' };
    const array<pair<char, Point2D>, 4> DIRS = {make_pair('U', Point2D(0,-1)), {'D', Point2D(0, 1)}, {'L', Point2D(-1, 0)}, {'R', Point2D(1, 0)}};
    const Point2D P_START = Point2D(0, 0);
    const Point2D P_GOAL = Point2D(3, 3);

    // List of possible goal paths
    vector<string> goalPaths = vector<string>();

    // Queue of points to visit (and current path to that point)
    queue<pair<Point2D, string>> pointsQueue;
    pointsQueue.push(make_pair(P_START, string()));

    MD5 md5 = MD5();
    while(!pointsQueue.empty())
    {
        const auto& cur = pointsQueue.front();
        const Point2D& curPoint = cur.first;
        const string& curPath = cur.second;

        const string hash = md5.digestString(passcode + curPath);
        for(size_t i = 0; i < DIRS.size(); ++i)
        {                  
            if(find(begin(OPEN_CHARS), end(OPEN_CHARS), hash[i]) != end(OPEN_CHARS))
            {
                const pair<char, Point2D>& dir = DIRS[i];
                const Point2D pointNext = (curPoint + dir.second);
                if(pointNext.IsWithinBounds(4,4))
                {
                    const string newPath = curPath + dir.first;
                    if(pointNext == P_GOAL)
                    {
                        goalPaths.push_back(newPath);
                    }
                    else
                    {
                        pointsQueue.push(make_pair(pointNext, newPath));
                    }
                }
            }
        }
        pointsQueue.pop();
    }

    os << "Path from (" << P_START.x << "," << P_START.y << ") to (" << P_GOAL.x << "," << P_GOAL.y << ")" << endl;
    if(goalPaths.size() == 0)
    {
        os << "Failed to find path" << endl;
    }
    else
    {
        string bestPath = goalPaths.front();
        for(const string path : goalPaths)
        {
            if(part == Part01 && path.size() < bestPath.size())
            {
                bestPath = path;
            }
            else if(part == Part02 && path.size() > bestPath.size())
            {
                bestPath = path;
            }
        }
        os << bestPath << endl;
        os << "Length: " << bestPath.length() << endl;
    }
}