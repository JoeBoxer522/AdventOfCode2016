#include <algorithm>
#include <vector>

#include "AdventMath.h"
#include "Days.h"

struct Triangle
{
    vector<int> points;
    int Size() const { return(points.size()); }
    int A() const { return points.at(0); }
    int B() const { return points.at(1); }
    int C() const { return points.at(2); }
    void Clear() {points.clear();}
};

void Day03::RunPart1()
{
    // Read input horizontally (one triangle per line)
    int numTriPossible = 0;
    int numTriTotal = 0;
    Triangle tri = Triangle();

    ifstream file("Input/Day03.txt");
    string arg;
    while(getline(file, arg, ' '))
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
                cout << "Error: Unable to convert \"" << arg << "\" to int\n";
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

    printf("\nPossible Triangles: %i", numTriPossible);
    printf("\nTotal Triangles: %i\n", numTriTotal);
}

void Day03::RunPart2()
{
    // Read input vertically in parallel (three triangles every three columns)
    int numTriPossible = 0;
    int numTriTotal = 0;

    const int MAX_TRI = 3;
    int triIdx = 0;
    Triangle triangles[MAX_TRI] = { Triangle(), Triangle(), Triangle() };

    ifstream file("Input/Day03.txt");
    string arg;
    while(getline(file, arg, ' '))
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
                cout << "Error: Unable to convert \"" << arg << "\" to int\n";
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

    printf("\nPossible Triangles: %i", numTriPossible);
    printf("\nTotal Triangles: %i\n", numTriTotal);
}