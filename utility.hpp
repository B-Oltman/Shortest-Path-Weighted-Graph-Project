#pragma once
#include <iostream>
#include <limits>

class Utility{
    public:
        // Clears input stream.
        static void ClearInStream();
};

void Utility::ClearInStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}