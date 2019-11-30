#pragma once
#include <cstddef>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "graph.hpp"

class Schedule{
    public:
        //Constructor - create new schedule from data files.
        Schedule(std::string stationData, std::string trainsData);
        //Destructor - destroy schedule
        //PrintComplete - print schedule for all stations
        //PrintSelected - print schedule for selected station
        //LookUpStationId - print station number for given station name
        //LookUpStationName - print station name for given station number
        //GetDirectRoute - returns whether there is a direct route from station A to station B
        //GetRoute - returns whether there is any route from station A to station B
        //TripLengthNoLayover - returns the shortest time to go from A to B with no layovers, else alert to no path
        //TripLengthYesLayover - returns the shortest time to go from A to B, layovers are allowed, else alert user to no path
        //TripLengthGivenTime - returns the shortest time to go from A to B when departing at a specific time only. 
    private:
        std::vector<std::vector<std::string>> stationLookupTable;
        void buildStationLookupTable(std::string stationData);
        //Build station data table.
};

Schedule::Schedule(std::string stationData, std::string trainsData)
{
    buildStationLookupTable(stationData);
}

void Schedule::buildStationLookupTable(std::string stationData)
{
    std::stringstream lineStream(stationData);
    
    std::string line;
    while(getline(lineStream, line))
    {
        std::stringstream tokenStream(line);
        std::string token;
        stationLookupTable.push_back({});
        while(tokenStream >> token)
        {
            stationLookupTable[stationLookupTable.size() - 1].push_back(token);
        }
    }
}

