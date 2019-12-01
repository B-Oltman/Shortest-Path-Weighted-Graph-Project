#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "station.hpp"

class StationGraph{
    public:
        StationGraph(std::vector<std::vector<std::string>> tripData);
    private:
        std::vector<std::vector<std::string>> tripDataTable;
};

StationGraph::StationGraph(std::vector<std::vector<std::string>> tripData)
{
    tripDataTable = tripData;
}



