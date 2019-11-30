#pragma once
#include <iostream>
#include <limits>

class Utility{
    public:
        // Clears input stream.
        static void ClearInStream();
        static bool CompareStringsNoCase(const std::string& s1, const std::string& s2);
};

void Utility::ClearInStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}

bool Utility::CompareStringsNoCase(const std::string& s1, const std::string& s2)
{
    if(s1.size() != s2.size())
    {
        return false;
    }

    for(int i = 0; i < s1.size(); i++)
    {
        // Treat space and _ as same character in string comparisons.
        if((s1[i] == '_' && s2[i] == ' ') || (s2[i] == '_' && s1[i] == ' '))
        {
            continue;
        }

        if(tolower(s1[i] != tolower(s2[i])))
        {
            return false;
        }
    }
    return true;
}