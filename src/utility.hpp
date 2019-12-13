#pragma once
#include <iostream>
#include <limits>

class Utility{
    public:
        // Clears input stream.
        static void ClearInStream();
        static bool CompareStringsNoCase(const std::string& s1, const std::string& s2);
        static int GetIntFromUser();
        static void PrintMainMenu();    
        static const int INF = std::numeric_limits<int>::max();
};

void Utility::ClearInStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}

void Utility::PrintMainMenu()
{
    std::cout << "========================================================================\n"
    << "READING RAILWAYS SCHEDULER\n"
    << "========================================================================\n"
    << "Options - (Enter number of your selected option)\n"
    << "(1) - Print full schedule\n"
    << "(2) - Print station schedule\n"
    << "(3) - Look up stationd id\n"
    << "(4) - Look up station name\n"
    << "(5) - Servie available\n"
    << "(6) - Nonstop service available\n"
    << "(7) - Find route (Shortest riding time)\n"
    << "(8) - Find route (Shortest overall travel time)\n"
    << "(9) - Find route (Shortest time, at specific departure time)\n"
    << "(0) - Exit\n";
}

int Utility::GetIntFromUser()
{
    int val;
    std::cin >> val;
    while(std::cin.fail())
    {
        std::cout << "Invalid input, try again: ";
        Utility::ClearInStream();
        std::cin >> val;
    }

    return val;
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