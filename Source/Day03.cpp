#include <algorithm>
#include <vector>

#include "AdventMath.h"
#include "Days.h"

struct Triangle
{
    vector<int> points;
    size_t Size() const { return(points.size()); }
    int A() const { return points.at(0); }
    int B() const { return points.at(1); }
    int C() const { return points.at(2); }
    void Clear() {points.clear();}
};

template <>
void Run<Day03>(Part part, istream& is, ostream& os)
{
    if(part == Part01)
    {
        // Read input horizontally (one triangle per line)
        int numTriPossible = 0;
        int numTriTotal = 0;
        Triangle tri = Triangle();

        string arg;
        while(getline(is, arg, ' '))
        {
            if(arg.length() > 0)
            {
                try
                {
                    int side = stoi(arg);
                    tri.points.push_back(side);
                }
                catch(...)
                {
                    os << "Error: Unable to convert \"" << arg << "\" to int\n";
                }
            }
            if(tri.Size() == 3)
            {
                if(tri.A() + tri.B() > tri.C() &&
                    tri.B() + tri.C() > tri.A() &&
                    tri.C() + tri.A() > tri.B())
                {
                    numTriPossible++;
                }
                numTriTotal++;
                tri.Clear();
            }
        }

        os << "Possible Triangles: " << numTriPossible << endl;
        os << "Total Triangles: " << numTriTotal << endl;
    }
    else if(part == Part02)
    {
        // Read input vertically in parallel (three triangles every three columns)
        int numTriPossible = 0;
        int numTriTotal = 0;

        const int MAX_TRI = 3;
        int triIdx = 0;
        Triangle triangles[MAX_TRI] = { Triangle(), Triangle(), Triangle() };

        string arg;
        while(getline(is, arg, ' '))
        {
            Triangle& tri = triangles[triIdx];
            if(arg.length() > 0)
            {
                try
                {
                    int side = stoi(arg);
                    tri.points.push_back(side);

                    // Prep to read input for next triangle
                    ++triIdx;
                    if(triIdx >= MAX_TRI)
                    {
                        triIdx = 0;
                    }
                }
                catch(...)
                {
                    os << "Error: Unable to convert \"" << arg << "\" to int\n";
                }
            }

            if(tri.Size() == 3)
            {
                if(tri.A() + tri.B() > tri.C() &&
                    tri.B() + tri.C() > tri.A() &&
                    tri.C() + tri.A() > tri.B())
                {
                    numTriPossible++;
                }
                numTriTotal++;
                tri.Clear();
            }
        }

        os << "Possible Triangles: " << numTriPossible << endl;
        os << "Total Triangles: " << numTriTotal << endl;
    }
}