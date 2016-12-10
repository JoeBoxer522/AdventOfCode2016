#include "Days.h"

#include "AdventMath.h"
#include "MD5.h"

const int PASSWORD_LEN = 8;

template <>
void Run<Day05>(Part part, istream& is, ostream& os)
{
    if(part == Part01)
    {
        MD5 md5;

        string doorPassword = string("????????");
        int pswCharIdx = 0;
        int incrementIdx = 0;

        string arg;
        while(getline(is, arg))
        {
            os << "HAcKing: " << doorPassword << "..." << endl;
            while(pswCharIdx < PASSWORD_LEN)
            {
                string s = arg + to_string(incrementIdx);
                string doorHash = md5.digestString(s.c_str());
                if(doorHash.substr(0, 5) == "00000")
                {
                    doorPassword[pswCharIdx] = doorHash.at(5);
                    os << "HAcKing: " << doorPassword << "..." << endl;
                    pswCharIdx++;
                }
                incrementIdx++;
            }
        }

        os << "Doorcode " << arg << " password is: " << doorPassword << endl;
    }
    else if(part == Part02)
    {
        MD5 md5;

        string doorPassword = string("????????");
        int foundChars = 0;
        int incrementIdx = 0;

        string arg;
        while(getline(is, arg))
        {
            os << "HAcKing: " << doorPassword << "..." << endl;
            while(foundChars < PASSWORD_LEN)
            {
                string s = arg + to_string(incrementIdx);
                string doorHash = md5.digestString(s.c_str());
                if(doorHash.substr(0, 5) == "00000" && isdigit(doorHash.at(5)))
                {
                    int passwordCharIdx = ToInt(doorHash.at(5));
                    if(passwordCharIdx < PASSWORD_LEN && doorPassword[passwordCharIdx] == '?')
                    {
                        doorPassword[passwordCharIdx] = doorHash.at(6);
                        os << "HAcKing: " << doorPassword << "..." << endl;
                        foundChars++;
                    }
                }
                incrementIdx++;
            }
        }

        os << "Doorcode " << arg << " password is: " << doorPassword << endl;
    }
}