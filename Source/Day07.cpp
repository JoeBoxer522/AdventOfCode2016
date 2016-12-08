#include "Days.h"

#include <vector>

const size_t ABBA_LEN = 4;
const size_t ABA_LEN = 3;

bool IsStringABBA(const string& s)
{
    if(s.length() == ABBA_LEN)
    {
        if(s[0] == s[3] && s[1] == s[2] && s[0] != s[1])
        {
            return(true);
        }
    }

    return(false);
}

bool IsStringABA(const string& s)
{
    if(s.length() == ABA_LEN)
    {
        if(s[0] == s[2] && s[0] != s[1])
        {
            return(true);
        }
    }

    return(false);
}

bool IsStringBAB(const string& s, const string& strABA)
{
    if(s.length() == ABA_LEN && IsStringABA(strABA))
    {
        if(s[0] == strABA[1] && s[1] == strABA[0] && s[2] == strABA[1])
        {
            return(true);
        }
    }

    return(false);
}

bool HasBABMatches(const string& s, const vector<string>& abaStrings)
{
    for(const string& abaString : abaStrings)
    {
        if(IsStringBAB(s, abaString))
        {
            return(true);
        }
    }
    return(false);
}

template <>
void Run<Day07>(Part part, istream& is, std::ostream& os)
{
    if(part == Part01)
    {
        size_t ipsTotal = 0;
        size_t ipsTLS = 0;

        string arg;
        while(getline(is, arg))
        {
            string testStr = string();
            bool withinBrackets = false;
            bool isTLS = false;

            for(size_t i = 0; i < arg.length(); ++i)
            {
                char c = arg[i];
                if(isalpha(c))
                {
                    testStr.push_back(c);   
                }
                else
                {
                    switch(c)
                    {
                        case '[':
                            withinBrackets = true;
                            break;
                        case ']':
                            withinBrackets = false;
                            break;
                    }
                    // Start a new test string to test what's within the brackets
                    testStr.clear();
                }

                // Maintain expected ABBA length, queue style FIFO
                while(testStr.length() > ABBA_LEN)
                {
                    testStr.erase(testStr.begin());
                }
                if(IsStringABBA(testStr))
                {
                    isTLS = !withinBrackets;
                    if(withinBrackets)
                    {
                        // ABBA within brackets is instant disqualifier, move on to next string
                        break;
                    }
                }
            }
            ++ipsTotal;
            if(isTLS)
            {
                ++ipsTLS;
            }
        }

        os << "Total IPs: " << ipsTotal << endl;
        os << "TLS IPs: " << ipsTLS << endl;
    }
    else if(part == Part02)
    {
        size_t ipsTotal = 0;
        size_t ipsSSL = 0;

        string arg;
        while(getline(is, arg))
        {
            string testStr = string();
            bool withinBrackets = false;
            vector<string> abaStrings = vector<string>();
            vector<string> abaStringsBracket = vector<string>();

            for(size_t i = 0; i < arg.length(); ++i)
            {
                char c = arg[i];
                if(isalpha(c))
                {
                    testStr.push_back(c);
                }
                else
                {
                    switch(c)
                    {
                        case '[':
                            withinBrackets = true;
                            break;
                        case ']':
                            withinBrackets = false;
                            break;
                    }
                    // Start a new test string to test what's within the brackets
                    testStr.clear();
                }

                // Maintain expected ABA length, queue style FIFO
                while(testStr.length() > ABA_LEN)
                {
                    testStr.erase(testStr.begin());
                }
                if(IsStringABA(testStr))
                {
                    if(withinBrackets)
                    {
                        abaStringsBracket.push_back(testStr);
                    }
                    else
                    {
                        abaStrings.push_back(testStr);
                    }
                }
            }
            ++ipsTotal;

            // Compare ABA strings within brackets to ABA strings outside of them to see if there are any BAB matches
            for(const string& abaStrBracket : abaStringsBracket)
            {
                if(HasBABMatches(abaStrBracket, abaStrings))
                {
                    ipsSSL++;
                    break;
                }            
            }
        }

        os << "Total IPs: " << ipsTotal << endl;
        os << "SSL IPs: " << ipsSSL << endl;
    }
}