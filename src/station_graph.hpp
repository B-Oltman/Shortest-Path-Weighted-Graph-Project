#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "station.hpp"

class StationGraph{
    public:
        StationGraph(std::vector<std::vector<std::string>> const tripData, int stationsCount);
        ~StationGraph();
        bool DirectPathExists(int station1ID, int station2ID);
        Station GetStationFromDepartGraph(int stationID);
        Station GetStationFromArrivalGraph(int stationID);
    private:
        const int stationCount;
        //Departure graph is the standard graph used for operations concerned with going from a starting node to a
        // destination node, this is the majority of the functions in the program.
        std::vector<Station>* departuresGraphList;
        // Arrivals graph is an inverted version of the graph so that arrivals to a station can be
        // looked up easily. Only used for a few functions in the program.
        std::vector<Station>* arrivalsGraphList;
        void BuildDeparturesGraph(std::vector<std::vector<std::string>> tripData);
        void BuildArrivalsGraph(std::vector<std::vector<std::string>> tripData);
};

StationGraph::StationGraph(std::vector<std::vector<std::string>> const tripDataTable, int stationsCount) : stationCount(stationsCount)
{
    BuildDeparturesGraph(tripDataTable);
    BuildArrivalsGraph(tripDataTable);
}

StationGraph::~StationGraph()
{
    if(departuresGraphList) delete departuresGraphList;
    if(arrivalsGraphList) delete arrivalsGraphList;
}

void StationGraph::BuildDeparturesGraph(std::vector<std::vector<std::string>> tripDataTable)
{
    // Use a temporary table to hold all trips so that they
    // can be passed into station constructor.
    // We don't want to allow the addition of new graph elements after construction because we are pre-computing shortest paths
    // so not making any public functions to add nodes or edges.
    departuresGraphList = new std::vector<Station>;

    std::vector<std::vector<Trip>> tempTripTable;
    for(int i = 0; i < stationCount; i++)
    {
        tempTripTable.push_back({});
    }

    //Add the trip data to tempTripTable array
    for(int i = 0; i < tripDataTable.size(); i++)
    {
        int startID = stoi(tripDataTable[i][0]) - 1;
        int destinationID = stoi(tripDataTable[i][1]);
        int arrivalTime = stoi(tripDataTable[i][3]);
        int departureTime = stoi(tripDataTable[i][2]);
        int travelTimeMins = (arrivalTime - departureTime);
        tempTripTable[startID].push_back({destinationID, departureTime, arrivalTime, travelTimeMins});
    }

    //Construct the stations and add trips to graph.
    for(int i = 0; i < stationCount; i++)
    {
        departuresGraphList->push_back({i + 1, tempTripTable[i]});
    }
}

void StationGraph::BuildArrivalsGraph(std::vector<std::vector<std::string>> tripDataTable)
{
    arrivalsGraphList = new std::vector<Station>;

    std::vector<std::vector<Trip>> tempTripTable;
    for(int i = 0; i < stationCount; i++)
    {
        tempTripTable.push_back({});
    }

    //Add the trip data to tempTripTable array
    for(int i = 0; i < tripDataTable.size(); i++)
    {
        int startID = stoi(tripDataTable[i][1]) - 1;
        int destinationID = stoi(tripDataTable[i][0]);
        int arrivalTime = stoi(tripDataTable[i][2]);
        int departureTime = stoi(tripDataTable[i][3]);
        int travelTimeMins = (arrivalTime - departureTime);
        tempTripTable[startID].push_back({destinationID, departureTime, arrivalTime, travelTimeMins});
    }

    //Construct the stations and add trips to graph.
    for(int i = 0; i < stationCount; i++)
    {
        arrivalsGraphList->push_back({i + 1, tempTripTable[i]});
    }
}

Station StationGraph::GetStationFromDepartGraph(int stationID)
{
    int iDAsZeroIndex = stationID - 1;
    if(iDAsZeroIndex < departuresGraphList->size() && iDAsZeroIndex >= 0)
    {
        return (*departuresGraphList)[iDAsZeroIndex];
    }
    else
    {
        // return invalid station if bad station ID
        return Station({-1, {}});
    } 
}

// Duplication of code between two graph types. Might want to pull this out to be more
// generic.
Station StationGraph::GetStationFromArrivalGraph(int stationID)
{
    int iDAsZeroIndex = stationID - 1;
    if(iDAsZeroIndex < arrivalsGraphList->size() && iDAsZeroIndex >= 0)
    {
        return (*arrivalsGraphList)[iDAsZeroIndex];
    }
    else
    {
        // return invalid station if bad station ID
        return Station({-1, {}});
    } 
}

bool StationGraph::DirectPathExists(int station1ID, int station2ID)
{
    for(int i = 0; i < GetStationFromDepartGraph(station1ID).GetTripCount(); i++)
    {
        if(GetStationFromDepartGraph(station1ID).GetTrip(i).destinationID == station2ID)
        {
            return true;
        }
    }
    return false;
}



