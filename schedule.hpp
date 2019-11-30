#pragma once
#include <cstddef>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "graph.hpp"
#include "utility.hpp"

class Schedule{
    public:
        //Constructor - create new schedule from data files.
        Schedule(std::string stationData, std::string trainsData);
        //Destructor - destroy schedule
        //PrintComplete - print schedule for all stations
        //PrintSelected - print schedule for selected station
        //LookUpStationId - print station number for given station name
        //LookUpStationName - print station name for given station number
        void LookUpStationName();
        //GetDirectRoute - returns whether there is a direct route from station A to station B
        //GetRoute - returns whether there is any route from station A to station B
        //TripLengthNoLayover - returns the shortest time to go from A to B with no layovers, else alert to no path
        //TripLengthYesLayover - returns the shortest time to go from A to B, layovers are allowed, else alert user to no path
        //TripLengthGivenTime - returns the shortest time to go from A to B when departing at a specific time only. 
    private:
        std::vector<std::vector<std::string>> stationLookupTable;

        // Builds a lookup table to map station id to station name.
        void BuildStationLookupTable(std::string stationData);
};

Schedule::Schedule(std::string stationData, std::string trainsData)
{
    BuildStationLookupTable(stationData);
}

void Schedule::LookUpStationName()
{
    int stationID;
    std::cout << "Enter station id: ";
    std::cin >> stationID;

    bool noMatch = true;
    for(int i = 0; i < stationLookupTable.size() && noMatch; i++)
    {
        if(std::stoi(stationLookupTable[i][0]) == stationID)
        {
            std::cout << "Station " << stationLookupTable[i][0] << " is " << 
            stationLookupTable[i][1] << std::endl;
            noMatch = false;
        }
    }
    if(noMatch)
    {
        std::cout <<"Invalid station id (enter value betweeen 1 and " << stationLookupTable.size() <<")\n";
    }

    //Clear input buffer
    Utility::ClearInStream();
}

void Schedule::BuildStationLookupTable(std::string stationData)
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

