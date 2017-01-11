#include "Days.h"
#include <map>
#include <algorithm>
#include <queue>
#include <vector>

struct Room
{
    char name;
    pair<int, int> pos;

    Room() :
        name('0'), pos(make_pair(0,0))
    {}

    Room(char n, pair<int, int> p) :
        name(n), pos(p)
    {}

    bool operator<(const Room& r) const
    {
        return name < r.name;
    }
};

bool IsDataPosValid(const pair<int, int>& p, const vector<vector<char>>& nodes)
{
    int x = p.first;
    int y = p.second;
    if(x >= 0 && y >= 0 && x < nodes.size() && y < nodes[x].size())
    {
        // Within range and a valid spot
        return(isdigit(nodes[x][y]) || nodes[x][y] == '.');
    }
    return(false);
}

// A*
int GetDist(const pair<int, int>& START, const pair<int, int>& GOAL, const vector<vector<char>>& nodes)
{
    static const vector<pair<int, int>> DIRS({ make_pair(0,-1),{ 0,1 },{ -1,0 },{ 1,0 } });

    // Queue of points to visit
    queue<pair<int, int>> pointsQueue = queue<pair<int, int>>();
    pointsQueue.push(START);

    // Map of points visited and the distance from start to that point
    map<pair<int, int>, int> pointPathMap = map<pair<int, int>, int>();
    pointPathMap.emplace(START, 0);

    while(!pointsQueue.empty() && pointPathMap.find(GOAL) == pointPathMap.end())
    {
        const pair<int, int>& pointCur = pointsQueue.front();
        for(const pair<int, int>& dir : DIRS)
        {
            const pair<int, int> pointNext = make_pair(pointCur.first + dir.first, pointCur.second + dir.second);
            if(IsDataPosValid(pointNext, nodes) && pointPathMap.find(pointNext) == pointPathMap.end())
            {
                pointsQueue.push(pointNext);
                const int searchDist = pointPathMap[pointCur];
                pointPathMap.emplace(pointNext, searchDist + 1);
            }
        }
        pointsQueue.pop();
    }

    if(pointPathMap.find(GOAL) != pointPathMap.end())
    {
        return pointPathMap[GOAL];
    }
    else
    {
        return -1;
    }
}

int TSM_Brute(vector<Room> rooms, const vector<vector<char>>& nodes, string& bestPath, bool part2)
{
    // Map of room path and the path distance
    map<string, int> pathDistMap;
    for(size_t i = 0; i < rooms.size(); ++i)
    {
        for(size_t j = 0; j < rooms.size(); ++j)
        {
            if(i != j)
            {
                const Room& r1 = rooms[i];
                const Room& r2 = rooms[j];
                string rPath1 = string() + r1.name + r2.name;
                string rPath2 = string() + r2.name + r1.name;
                if( pathDistMap.find(rPath1) == pathDistMap.end() &&
                    pathDistMap.find(rPath2) == pathDistMap.end())
                {
                    int dist = GetDist(r1.pos, r2.pos, nodes);
                    pathDistMap[rPath1] = pathDistMap[rPath2] = dist;
                }
            }
        }
    }

    // Search every permutation of possible paths, using the sum of each step to create a total
    // Always start from first room
    // Always end in last room for part 2
    bestPath = "";
    int bestDist = int(1e9);
    do 
    {
        string path = string() + rooms[0].name;
        int dist = 0;
        for(int i=1; i < rooms.size(); ++i)
        {
            string roomPath = string() + rooms[i-1].name + rooms[i].name;
            dist += pathDistMap[roomPath];
            path.push_back(rooms[i].name);
        }
        if(dist < bestDist)
        {
            bestDist = dist;
            bestPath = path;
        }
    }
    while(next_permutation(rooms.begin() + 1, rooms.end() - (part2 ? 1 : 0)));

    return bestDist;
}

int TSM_NearNeighbor(vector<Room> rooms, const vector<vector<char>>& nodes, string& bestPath, bool part2)
{
    // "Greedy" algorithm that finds the best possible case at each step and returns the result
    bestPath = "";
    int totalDist = 0;

    // Always start from first room
    bestPath.push_back(rooms.front().name);
    while(rooms.size() > 1)
    {
        // Start from current room
        const pair<int, int>& pStart = rooms.front().pos;

        // Find next closest room
        int rDistBest = int(1e9);
        int rIdxBest = -1;
        for(size_t i=1; i<rooms.size(); ++i)
        {
            // For part 2, ignore last room until it's only remainder
            if(!part2 || i < rooms.size() - 1 || rooms.size() <= 2)
            {
                const pair<int, int>& pGoal = rooms[i].pos;
                int rDist = GetDist(pStart, pGoal, nodes);
                if(rDist > 0 && rDist < rDistBest)
                {
                    rDistBest = rDist;
                    rIdxBest = int(i);
                }
            }
        }
        totalDist += rDistBest;
        bestPath.push_back(rooms[rIdxBest].name);

        // Erase current room and move found room to the front
        rooms.erase(rooms.begin(), rooms.begin() + 1);
        iter_swap(rooms.begin() + (rIdxBest - 1), rooms.begin());
    }
    
    return totalDist;
}

// Completed using two approaches to Traveling Salesman Problem
template <>
void Run<Day24>(Part part, istream& is, ostream& os)
{
    vector<Room> rooms;
    vector<vector<char>> nodes;

    string arg;
    int y = 0;
    while(getline(is, arg))
    {
        nodes.push_back(vector<char>()); // num rows
        for(int x = 0; x < arg.length(); ++x)
        {
            char c = arg[x];
            if(isdigit(c))
            {
                Room room(c, make_pair(y, x));
                if(c == '0')
                {
                    rooms.insert(rooms.begin(), room);
                }
                else
                {
                    rooms.push_back(room);
                }
            }
            if(nodes[y].size() <= x) // num cols
            {
                nodes[y].resize(x + 1);
            }
            nodes[y][x] = c;
        }
        ++y;
    }
    bool part2 = (part == Part02);
    if(part2)
    {
        // Part2 wants to go back to start after path is found
        rooms.push_back(rooms.front());
    }

    string bestPath = "";
    int totalDist = 0;
    
    totalDist = TSM_Brute(rooms, nodes, bestPath, part2);
    os << "Brute method:" << endl;
    os << "Path " << bestPath << " - Total Dist: " << totalDist << endl << endl;

    totalDist = TSM_NearNeighbor(rooms, nodes, bestPath, part2);
    os << "Nearest Neighbor:" << endl;
    os << "Path " << bestPath << " - Total Dist: " << totalDist << endl;
}