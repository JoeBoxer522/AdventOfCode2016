#include "Days.h"

#include "AdventMath.h"
#include "MD5.h"

const string DOOR_CODE = "reyedfim"; // Test was ABC
const int PASSWORD_LEN = 8;

void Day05::RunPart1()
{
    MD5 md5;

    string doorPassword = string("????????");
    int pswCharIdx = 0;
    int incrementIdx = 0;

    printf("\nHAcKing: %s...\n", doorPassword.c_str());
    while(pswCharIdx < PASSWORD_LEN)
    {
        string s = DOOR_CODE + to_string(incrementIdx);
        string doorHash = md5.digestString(s.c_str());
        if(doorHash.substr(0,5) == "00000")
        {   
            doorPassword[pswCharIdx] = doorHash.at(5);
            printf("HAcKing: %s...\n", doorPassword.c_str());
            pswCharIdx++;
        }
        incrementIdx++;
    }
    
    printf("Doorcode %s password is: %s\n", DOOR_CODE.c_str(), doorPassword.c_str());
}

void Day05::RunPart2()
{
    MD5 md5;

    string doorPassword = string("????????");
    int foundChars = 0;
    int incrementIdx = 0;

    printf("\nHAcKing: %s...\n", doorPassword.c_str());
    while(foundChars < PASSWORD_LEN)
    {
        string s = DOOR_CODE + to_string(incrementIdx);
        string doorHash = md5.digestString(s.c_str());
        if(doorHash.substr(0, 5) == "00000" && isdigit(doorHash.at(5)))
        {
            int passwordCharIdx = ToInt(doorHash.at(5));
            if(passwordCharIdx < PASSWORD_LEN && doorPassword[passwordCharIdx] == '?')
            {
                doorPassword[passwordCharIdx] = doorHash.at(6);
                printf("HAcKing: %s...\n", doorPassword.c_str());
                foundChars++;
            }
        }
        incrementIdx++;
    }

    printf("Doorcode %s password is: %s\n", DOOR_CODE.c_str(), doorPassword.c_str());
}