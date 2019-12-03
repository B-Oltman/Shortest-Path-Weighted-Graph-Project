#pragma once
#include <cstddef>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "station_graph.hpp"
#include "utility.hpp"

class Schedule{
    public:
        //Constructor - create new schedule from data files.
        Schedule(std::string stationData, std::string trainsData);
        //Destructor - destroy schedule
        ~Schedule();
        //PrintComplete - print schedule for all stations
        //PrintSelected - print schedule for selected station
        //LookUpStationId - print station number for given station name
        void LookUpStationId();

        //Print station name for given station number
        void LookUpStationName();
        std::string SimpleStationNameLookup(int stationID);

        //GetDirectRoute - returns whether there is a direct route from station A to station B
        void GetDirectRoute();
        //GetRoute - returns whether there is any route from station A to station B
        //TripLengthNoLayover - returns the shortest time to go from A to B with no layovers, else alert to no path
        //TripLengthYesLayover - returns the shortest time to go from A to B, layovers are allowed, else alert user to no path
        //TripLengthGivenTime - returns the shortest time to go from A to B when departing at a specific time only. 
    private:
        std::vector<std::vector<std::string>> stationLookupTable;
        std::vector<std::vector<std::string>> tripDataTable;
        StationGraph* stationGraph;
        // Builds a lookup table to map station id to station name.
        void BuildStationLookupTable(std::string stationData);
        void BuildTripDataTable(std::string trainsData);
        std::pair<int, int> PromptStationID() const;
        
};

Schedule::Schedule(std::string stationData, std::string trainsData)
{
    BuildStationLookupTable(stationData);
    BuildTripDataTable(trainsData);
    stationGraph = new StationGraph(tripDataTable, stationLookupTable.size());
}

Schedule::~Schedule()
{
    if(stationGraph)
    {
        delete stationGraph;
    }
}

void Schedule::LookUpStationId()
{
    std::string stationName;
    std::cout << "Enter station name: ";
    Utility::ClearInStream();
    getline(std::cin, stationName);

    bool noMatch = true;
    for(int i = 0; i < stationLookupTable.size() && noMatch; i++)
    {
        if(Utility::CompareStringsNoCase(stationLookupTable[i][1], stationName))
        {
            std::string possessive = tolower(stationName[stationName.size() - 1]) == 's' ? "'" : "'s"; 
            std::cout << stationLookupTable[i][1] << possessive << " station id is " << 
            stationLookupTable[i][0] << std::endl;
            noMatch = false;
        }
    }
    if(noMatch)
    {
        std::cout <<"Invalid station name.\n";
    }

    std::cout <<"Press Enter to continue.";
    Utility::ClearInStream();
}

std::string Schedule::SimpleStationNameLookup(int stationID)
{
    if(stationID > 0 && stationID <= stationLookupTable.size())
    {
           return stationLookupTable[stationID - 1][1];
    }
    else
    {
        return "INVALID";
    }
}

void Schedule::LookUpStationName()
{
    int stationID;
    std::cout << "Enter station id: ";
    std::cin >> stationID;

    if(stationID > 0 && stationID <= stationLookupTable.size())
    {
        //stationID - 1 maps the input to the corresponding vector entry.
        std::cout << "Station " << stationLookupTable[stationID - 1][0] << " is " << 
            stationLookupTable[stationID - 1][1] << std::endl;
    }
    else
    {
        std::cout <<"Invalid station id (enter value betweeen 1 and " << stationLookupTable.size() <<")\n";
    }
  
    //Clear input buffer
    Utility::ClearInStream();

    std::cout << "Press Enter to continue.";
    bool wait = true;
    while(wait)
    {
        if(std::cin.get() == '\n')
        {
            wait = false;
        }
    }
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

void Schedule::BuildTripDataTable(std::string trainsData)
{
    std::stringstream lineStream(trainsData);
    
    std::string line;
    while(getline(lineStream, line))
    {
        std::stringstream tokenStream(line);
        std::string token;
        tripDataTable.push_back({});
        while(tokenStream >> token)
        {
            tripDataTable[tripDataTable.size() - 1].push_back(token);
        }
    }
}

std::pair<int, int> Schedule::PromptStationID() const
{  
    std::cout << "Enter departure station id: ";
    int departID = Utility::GetIntFromUser();
    while(stationGraph->GetStation(departID).StationIsValid() == false)
    {
        std::cout << "Departure station id invalid, try again: ";
        departID = Utility::GetIntFromUser();
    }

    std::cout << "Enter destination station id: ";
    int destID = Utility::GetIntFromUser();
    while(stationGraph->GetStation(destID).StationIsValid() == false)
    {
        std::cout << "Destination station id invalid, try again: ";
        destID = Utility::GetIntFromUser();
    }

    return {departID, destID};
}

void Schedule::GetDirectRoute()
{
    std::pair<int, int> stationPair = PromptStationID();

    if(stationGraph->DirectPathExists(stationPair.first, stationPair.second))
    {

        std::cout << "Nonstop service is available from " << SimpleStationNameLookup(stationPair.first) << 
        " to " << SimpleStationNameLookup(stationPair.second) << std::endl;
    }
    else
    {
        std::cout << "Nonstop service is NOT available from " << SimpleStationNameLookup(stationPair.first) << 
        " to " << SimpleStationNameLookup(stationPair.second) << std::endl;
    }
}
