#include "Days.h"
#include <regex>
#include <vector>

static int dist(int x1, int x2, int y1, int y2)
{
    return (abs(x1 - x2) + abs(y1 - y2));
}
static int dist(pair<int,int> p1, pair<int,int> p2)
{
    return dist(p1.first, p2.first, p1.second, p2.second);
}

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
        if(dataUsed > 100)
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
            if(nodes.size() <= y) // num rows
            {
                nodes.push_back(vector<Node>());
            }
            if(nodes[y].size() <= x) // num cols
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
        for(int i = 0; i < SQUARE; ++i)
        {
            const Node& n1 = nodes[i / WIDTH][i - (WIDTH * (i / WIDTH))];
            for(int j = 0; j < SQUARE; ++j)
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
        // Takes 5 moves to transition goal data from one node to an adjacent (assuming capacity is available)
        const int MOVE_COST = 5;

        int distGoalEmpty = int(1e9);
        
        pair<int, int> pStart = make_pair(0, 0);
        pair<int, int> pGoal = make_pair(0, int(WIDTH - 1));

        // Find empty data node closest to goal
        for(int x = 0; x < nodes.size(); ++x)
        {
            for(int y = 0; y < nodes[x].size(); ++y)
            {
                nodes[x][y].Print(os);
                if(nodes[x][y].dataUsed == 0)
                {
                    int emptyDist = dist(pGoal.first, x, pGoal.second, y);
                    if(emptyDist < distGoalEmpty)
                    {
                        distGoalEmpty = emptyDist;
                    }
                }
            }
            os << endl;
        }

        // distGoalEmpty = 58
        // distGoalStart = 28
        // Both of these need to case for hitting 'walls'
        // A* ?
        int distGoalStart = dist(pStart, pGoal) - 1;
        int totalMoves = (distGoalStart * MOVE_COST) + distGoalEmpty;
        os << "Shortest path takes " << totalMoves << " moves" << endl;
    }
}