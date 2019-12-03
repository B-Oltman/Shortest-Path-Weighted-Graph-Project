#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "station.hpp"

class StationGraph{
    public:
        StationGraph(std::vector<std::vector<std::string>> tripData, int stationsCount);
        bool DirectPathExists(int station1ID, int station2ID);
        const Station GetStation(int stationID);
    private:
        const int stationCount;
        std::vector<Station> stationList;
        void BuildGraph(std::vector<std::vector<std::string>> tripData);
        void DebugTestPrint();
};

StationGraph::StationGraph(std::vector<std::vector<std::string>> tripDataTable, int stationsCount) : stationCount(stationsCount)
{
    BuildGraph(tripDataTable);
}

void StationGraph::BuildGraph(std::vector<std::vector<std::string>> tripDataTable)
{
    // Use a temporary table to hold all trips so that they
    // can be passed into station constructor.
    // We don't want to allow the addition of new graph elements after construction because we are pre-computing shortest paths
    // so not making any public functions to add nodes or edges.
    std::vector<std::vector<Trip>> tempTripTable;
    for(int i = 0; i < stationCount; i++)
    {
        //stationList.push_back(i + 1);
        tempTripTable.push_back({});
    }

    //Add the trip data to tempTripTable array
    for(int i = 0; i < tripDataTable.size(); i++)
    {
        int startID = stoi(tripDataTable[i][0]) - 1;
        int destinationID = stoi(tripDataTable[i][1]);
        int travelTimeMins = stoi(tripDataTable[i][3]) - stoi(tripDataTable[i][2]);
        tempTripTable[startID].push_back({destinationID, travelTimeMins});
    }

    //Construct the stations and add trips to graph.
    for(int i = 0; i < stationCount; i++)
    {
        stationList.push_back({i + 1, tempTripTable[i]});
    }

    //DebugTestPrint();
}

const Station StationGraph::GetStation(int stationID)
{
    int iDAsZeroIndex = stationID - 1;
    if(iDAsZeroIndex < stationList.size() && iDAsZeroIndex >= 0)
    {
        return stationList[iDAsZeroIndex];
    }
    else
    {
        // return invalid station if bad station ID
        return Station({-1, {}});
    } 
}

bool StationGraph::DirectPathExists(int station1ID, int station2ID)
{
    for(int i = 0; i < GetStation(station1ID).GetTripCount(); i++)
    {
        if(GetStation(station1ID).GetTrip(i).destinationID == station2ID)
        {
            return true;
        }
    }
    return false;
}

void StationGraph::DebugTestPrint()
{
    for(int i = 0; i < stationCount; i++)
    {
        std::cout << "Station: " << stationList[i].GetID() << " trips - ";
        bool multiplePass = false;
        for(int j = 0; j < stationList[i].GetTripCount(); j++)
        {
            if(multiplePass)
            {
                std::cout << std::endl;
                std::cout << "Station: " << stationList[i].GetID() << " trips - ";
            }
            std::cout << "Destination: " << stationList[i].GetTrip(j).destinationID << " - Travel Time: " << stationList[i].GetTrip(j).travelTimeMins;
            multiplePass = true;
        }
        std::cout << std::endl;
    }
}



