#include "Days.h"
#include <regex>
#include <vector>

static const regex RX_MICROCHIP{ R"((\w+)-compatible microchip)" };
static const regex RX_GENERATOR{ R"((\w+) generator)" };
static vector<struct FloorData> floors = vector<struct FloorData>();
static size_t maxChips(0), maxGens(0);

struct FloorData
{
    FloorData()
    {}

    size_t NumChips() const { return chips.size(); }
    size_t NumGens() const { return gens.size(); }
    size_t NumItems() const { return NumChips() + NumGens(); }

    void AddChip(const char c)
    {
        chips.push_back(c);
    }
    void AddGen(const char c)
    {
        gens.push_back(c);
    }
    void CopyFrom(const FloorData& floor)
    {
        for(char c : floor.chips)
        {
            chips.push_back(c);
        }
        for(char g : floor.gens)
        {
            gens.push_back(g);
        }
    }
    void Clear()
    {
        chips.clear();
        gens.clear();
    }
    void Print(int floorNum, ostream& os, bool hasElevator) const
    {
        os << "F" << floorNum << " ";
        os << (hasElevator ? " E " : " . ");
        for(int i=0; i<static_cast<int>(max(maxChips, maxGens)); ++i)
        {
            if(static_cast<int>(gens.size()) > i)
            {
                os << " " << gens[i] << "G";
            }
            else
            {
                os << " . ";
            }
            if(static_cast<int>(chips.size()) > i)
            {
                os << " " << chips[i] << "M";
            }
            else
            {
                os << " . ";
            }
        }
        os << endl;
    }

    vector<char> chips;
    vector<char> gens;
};

void PrintFloors(ostream& os, size_t eIdx)
{
    for(int i = int(floors.size()) - 1; i >= 0; --i)
    {
        const FloorData& floor = floors[i];
        floor.Print(i + 1, os, eIdx == i);
    }
    os << endl;
}

void InitFloorData(istream& is, string extraFloor1)
{
    floors.clear();
    maxChips = 0;
    maxGens = 0;

    string arg;
    while(getline(is, arg))
    {
        if(maxChips == 0 && maxGens == 0)
        {
            arg.append(extraFloor1);
        }
        FloorData floor = FloorData();

        smatch match;
        const sregex_token_iterator End;
        for(sregex_token_iterator i(arg.begin(), arg.end(), RX_MICROCHIP, 1); i != End; ++i)
        {
            floor.AddChip(toupper(string(*i)[0]));
            ++maxChips;
        }
        for(sregex_token_iterator i(arg.begin(), arg.end(), RX_GENERATOR, 1); i != End; ++i)
        {
            floor.AddGen(toupper(string(*i)[0]));
            ++maxGens;
        }

        floors.push_back(floor);
    }
}

template <>
void Run<Day11>(Part part, istream& is, ostream& os)
{    
    floors.empty();

    string extraFloor1 = "";
    if(part == Part02)
    {
        // Part 2 Input:
        extraFloor1 = "An elerium generator, an elerium-compatible microchip, a dilithium generator, a dilithium-compatible microchip.";
    }

    InitFloorData(is, extraFloor1);
    
    size_t eIdx = 0;
    size_t moves = 0;
    const FloorData& topFloor = floors.back();
    while(topFloor.NumItems() < maxChips + maxGens)
    {
        os << "Moves: " << moves << endl;
        PrintFloors(os, eIdx);

        // Formula for amount of moves required to move 'n' items
        moves += 2 * floors[eIdx].NumItems() - 3;

        // Move data up one floor
        floors[eIdx + 1].CopyFrom(floors[eIdx]);
        floors[eIdx].Clear();

        // Increment elevator
        eIdx++;
    }

    os << "Total Moves: " << moves << endl;
}