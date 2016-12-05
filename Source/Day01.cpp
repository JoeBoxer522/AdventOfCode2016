#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Days.h"
#include "Vector2D.h"

bool GetDirDist(const string arg, char& dir, int& dist)
{
    size_t i = 0;
    while(i < arg.length() - 1)
    {
		dir = arg[i];
        ++i;
        if(dir == 'L' || dir == 'R')
        {
            string subStr = arg.substr(i);
            try
            {
                dist = stoi(subStr);
                return(true);
            }
            catch(...)
            {
                cout << "Error: Unable to convert \"" << subStr << "\" to int\n";
            }
        }
	}
    return(false);
}

float Signed2DTriArea(const Vector2D& a, const Vector2D& b, const Vector2D& c)
{
	return float((a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x));
}

bool Test2DSegment(const Vector2D& a, const Vector2D& b, const Vector2D& c, const Vector2D& d, float &t, Vector2D& p)
{
	// signs of areas correspond to which side of ab points c and d are
	float a1 = Signed2DTriArea(a, b, d); // Compute winding of abd (+ or -)
	float a2 = Signed2DTriArea(a, b, c); // To intersect, must have sign opposite of a1

	// If c and d are on different sides of ab, areas have different signs
	if(a1 * a2 < 0.0f) // require unsigned x & y values.
	{
		float a3 = Signed2DTriArea(c, d, a); // Compute winding of cda (+ or -)
		float a4 = a3 + a2 - a1; // Since area is constant a1 - a2 = a3 - a4, or a4 = a3 + a2 - a1

		// Points a and b on different sides of cd if areas have different signs
		if(a3 * a4 < 0.0f)
		{
			// Segments intersect. Find intersection point along L(t) = a + t * (b - a).
			t = a3 / (a3 - a4);
			p = a + t * (b - a); // the point of intersection
			return(true);
		}
	}

	// Segments not intersecting or collinear
	return(false);
}

void Day01::RunPart1()
{
	//L3, R2, L5, R1, L1, L2, L2, R1, R5, R1, L1, L2, R2, R4, L4, L3, L3, R5, L1, R3, L5, L2, R4, L5, R4, R2, L2, L1, R1, L3, L3, R2, R1, L4, L1, L1, R4, R5, R1, L2, L1, R188, R4, L3, R54, L4, R4, R74, R2, L4, R185, R1, R3, R5, L2, L3, R1, L1, L3, R3, R2, L3, L4, R1, L3, L5, L2, R2, L1, R2, R1, L4, R5, R4, L5, L5, L4, R5, R4, L5, L3, R4, R1, L5, L4, L3, R5, L5, L2, L4, R4, R4, R2, L1, L3, L2, R5, R4, L5, R1, R2, R5, L2, R4, R5, L2, L3, R3, L4, R3, L2, R1, R4, L5, R1, L5, L3, R4, L2, L2, L5, L5, R5, R2, L5, R1, L3, L2, L2, R3, L3, L4, R2, R3, L1, R2, L5, L3, R4, L4, R4, R3, L3, R1, L3, R5, L5, R1, R5, R3, L1

	// N == (0, -1)
	// E == (1, 0)
	// S == (0, 1)
	// W == (-1, 0)

	Vector2D current(0, 0);
	Vector2D facing(0, -1);

    ifstream file("Input/Day01.txt");
    string arg;
    while(getline(file, arg, ','))
	{
		char dir = ' ';
		int dist = 0;
		if(GetDirDist(arg, dir, dist))
		{
			if(dir == 'L')
			{
				facing.RotateByDegrees(-90.0);
			}
			else if(dir == 'R')
			{
				facing.RotateByDegrees(90.0);
			}

			current += (facing * dist);
		}
	}

    printf("\nTotal Dist: %f", current.Length());
    printf("\nTotal Blocks: %i\n", abs(current.x) + abs(current.y));
}

void Day01::RunPart2()
{
	//L3, R2, L5, R1, L1, L2, L2, R1, R5, R1, L1, L2, R2, R4, L4, L3, L3, R5, L1, R3, L5, L2, R4, L5, R4, R2, L2, L1, R1, L3, L3, R2, R1, L4, L1, L1, R4, R5, R1, L2, L1, R188, R4, L3, R54, L4, R4, R74, R2, L4, R185, R1, R3, R5, L2, L3, R1, L1, L3, R3, R2, L3, L4, R1, L3, L5, L2, R2, L1, R2, R1, L4, R5, R4, L5, L5, L4, R5, R4, L5, L3, R4, R1, L5, L4, L3, R5, L5, L2, L4, R4, R4, R2, L1, L3, L2, R5, R4, L5, R1, R2, R5, L2, R4, R5, L2, L3, R3, L4, R3, L2, R1, R4, L5, R1, L5, L3, R4, L2, L2, L5, L5, R5, R2, L5, R1, L3, L2, L2, R3, L3, L4, R2, R3, L1, R2, L5, L3, R4, L4, R4, R3, L3, R1, L3, R5, L5, R1, R5, R3, L1

	Vector2D current(0, 0);
	Vector2D facing(0, -1);
    Vector2D intersection(0, 0);
    bool foundIntersect = false;

	vector<Vector2D> visited = vector<Vector2D>();
	visited.push_back(current);

    ifstream file("Input/Day01.txt");
    string arg;
    while(getline(file, arg, ',') && !foundIntersect)
	{
		char dir = ' ';
		int dist = 0;
		if(GetDirDist(arg, dir, dist))
		{
			if(dir == 'L')
			{
				facing.RotateByDegrees(-90.0);
			}
			else if(dir == 'R')
			{
				facing.RotateByDegrees(90.0);
			}
            
			// Brute force
// 			Vector2D dest = current + (facing * dist);
// 			while(current != dest)
// 			{
// 				current += facing;
// 				if(find(visited.begin(), visited.end(), current) == visited.end())
// 				{
// 					visited.push_back(current);
// 				}
// 				else
// 				{
// 					// -6, 130
// 					// 136 blocks
//                     intersection = current;
//                     foundIntersect = true;
// 				}
// 			}

			// Line intersect
			current += (facing * dist);
			if(visited.size() > 2)
			{
				const size_t lastIdx = visited.size() - 1;
				const Vector2D& currentA = visited.at(lastIdx);
				const Vector2D& currentB = current;
				float t = 0.0f;

				// Test last created line segment against every other segment in the sequence
				for(size_t j = 0; j < lastIdx-1; ++j)
				{
					const Vector2D& TestA = visited.at(j);
					const Vector2D& TestB = visited.at(j+1);
					
					if(Test2DSegment(TestA, TestB, currentA, currentB, t, intersection))
					{
						// -6, 130
						// 136 blocks
                        foundIntersect = true;
					}
				}
			}
			visited.push_back(current);
		}
	}

    printf("\nFirst intersect at (%i, %i)", intersection.x, intersection.y);
    printf("\nTotal Blocks: %i", abs(intersection.x) + abs(intersection.y));
}
