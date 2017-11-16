#include "Days.h"
#include <regex>
#include <map>
#include <queue>
#include <vector>

const int LARGE_DATA = 100;

struct Node
{
    int dataUsed;
    int dataTotal;

    Node() : 
        dataUsed(0), dataTotal(0)
    {}

    Node(int inDataUsed, int inDataTotal) :
        dataUsed(inDataUsed), dataTotal(inDataTotal)
    {}

    int DataAvail() const { return (dataTotal - dataUsed); }
    bool CanMove(const Node& n2) const { return(dataUsed > 0 && dataUsed <= n2.DataAvail()); }
    void Print(ostream& os) const
    {
        if(dataUsed > LARGE_DATA)
        {
            os << "#";
        }
        else if(dataUsed > 0)
        {
            os << ".";
        }
        else
        {
            os << "_";
        }
        os << " ";
    }
};

bool IsDataPosValid(const pair<int,int>& p, const vector<vector<Node>>& nodes)
{
    int x = p.first;
    int y = p.second;
    if(x >= 0 && y >= 0 && x < static_cast<int>(nodes.size()) && y < static_cast<int>(nodes[x].size()))
    {
        return (nodes[x][y].dataUsed < LARGE_DATA);
    }
    return(false);
}

int GetDist(const pair<int,int>& START, const pair<int,int>& GOAL, const vector<vector<Node>>& nodes)
{
    const vector<pair<int,int>> DIRS({make_pair(0,-1), {0,1}, {-1,0}, {1,0}});
    
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

template <>
void Run<Day22>(Part part, istream& is, ostream& os)
{
    const regex RX_DATA(R"(/dev/grid/node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T\s+(\d+)%)");

    vector<vector<Node>> nodes;

    string arg;
    while(getline(is, arg))
    {
        smatch match;
        if(regex_match(arg, match, RX_DATA))
        {
            int x = stoi(match.str(1));
            int y = stoi(match.str(2));
            if(static_cast<int>(nodes.size()) <= y) // num rows
            {
                nodes.push_back(vector<Node>());
            }
            if(static_cast<int>(nodes[y].size()) <= x) // num cols
            {
                nodes[y].resize(x+1);
            }
            nodes[y][x] = Node(stoi(match.str(4)), stoi(match.str(3)));
        }
    }

    const size_t WIDTH = nodes[0].size();
    const size_t HEIGHT = nodes.size();
    const size_t SQUARE = WIDTH * HEIGHT;

    if(part == Part01)
    {
        size_t numPairs = 0;
        for(unsigned int i = 0; i < SQUARE; ++i)
        {
            const Node& n1 = nodes[i / WIDTH][i - (WIDTH * (i / WIDTH))];
            for(unsigned int j = 0; j < SQUARE; ++j)
            {
                const Node& n2 = nodes[j / WIDTH][j - (WIDTH * (j / WIDTH))];
                if(i != j && n1.CanMove(n2))
                {
                    ++numPairs;
                }
            }
        }

        os << "Total possible pairings: " << numPairs << endl;
    }
    else if(part == Part02)
    {        
        const pair<int, int> pStart = make_pair(0, 0);
        const pair<int, int> pGoal = make_pair(0, int(WIDTH - 1));

        // Find empty data node closest to goal
        int distGoalEmpty = int(1e9);
        for(int x = 0; x < static_cast<int>(nodes.size()); ++x)
        {
            for(int y = 0; y < static_cast<int>(nodes[x].size()); ++y)
            {
                nodes[x][y].Print(os);
                if(nodes[x][y].dataUsed == 0)
                {
                    int emptyDist = GetDist(pGoal, make_pair(x, y), nodes);
                    if(emptyDist < distGoalEmpty)
                    {
                        distGoalEmpty = emptyDist;
                    }
                }
            }
            os << endl;
        }

        // Takes 5 moves to transition goal data from one node to an adjacent (assuming capacity is available)
        const int MOVE_COST = 5;

        // Calculate distance to move empty slot near goal + distance to move goal data to start
        int distGoalStart = GetDist(pStart, pGoal, nodes) - 1;
        int totalMoves = distGoalEmpty + (distGoalStart * MOVE_COST);
        os << "Shortest path takes " << totalMoves << " moves" << endl;
    }
}