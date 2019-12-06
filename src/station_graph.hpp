#pragma once
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include "station.hpp"

class StationGraph{
    public:
        StationGraph(std::vector<std::vector<std::string>> const tripData, int stationsCount);
        ~StationGraph();
        bool DirectPathExists(int station1ID, int station2ID);
        bool PathExists(int startStationID, int targetStationID);
        Station GetStationFromDepartGraph(int stationID);
        Station GetStationFromArrivalGraph(int stationID);
        int GetVertexCount();
        void DebugTestPrintShortPaths();
    private:
        const int stationCount;
        //Departure graph is the standard graph used for operations concerned with going from a starting node to a
        // destination node, this is the majority of the functions in the program.
        std::vector<Station>* departuresGraphList;
        // Arrivals graph is an inverted version of the graph so that arrivals to a station can be
        // looked up easily. Only used for a few functions in the program.
        std::vector<Station>* arrivalsGraphList;
        std::vector<std::vector<int>>* shortestPathSequenceTable;
        void floyd_warshal_shortest_paths();
        void build_departures_graph(std::vector<std::vector<std::string>> tripData);
        void build_arrivals_graph(std::vector<std::vector<std::string>> tripData);

};

void StationGraph::DebugTestPrintShortPaths()
{
    for(int i = 0; i < stationCount; i++)
    {
        for(int j = 0; j < stationCount; j++)
        {
            std::cout << (*shortestPathSequenceTable).at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
}

StationGraph::StationGraph(std::vector<std::vector<std::string>> const tripDataTable, int stationsCount) : stationCount(stationsCount)
{
    build_departures_graph(tripDataTable);
    build_arrivals_graph(tripDataTable);
    floyd_warshal_shortest_paths();
}

StationGraph::~StationGraph()
{
    if(departuresGraphList) delete departuresGraphList;
    if(arrivalsGraphList) delete arrivalsGraphList;
    if(shortestPathSequenceTable) delete shortestPathSequenceTable;
}

void StationGraph::build_departures_graph(std::vector<std::vector<std::string>> tripDataTable)
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

void StationGraph::build_arrivals_graph(std::vector<std::vector<std::string>> tripDataTable)
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

void StationGraph::floyd_warshal_shortest_paths()
{
    const int INF = Utility::INF;
    // Construct adjacency matrix from adjacencyList. If value == INF, no path exists between start and end index.
    std::vector<std::vector<int>> distance(stationCount, std::vector<int>(stationCount, INF));
    // Sequence table to store shortest paths for future operations.
    shortestPathSequenceTable = new std::vector<std::vector<int>>(stationCount, std::vector<int>(stationCount, 0));

    for(int i = 0; i < stationCount; i++)
    {
        Station currentStation = (*departuresGraphList)[i];
        for(int j = 0; j < currentStation.GetTripCount(); j++)
        {
            int tripWeight = currentStation.GetTrip(j).travelTimeMins;
            std::cout << tripWeight << "-" << "-" << i << j << std::endl;
            int startID = i;
            int destinationID = currentStation.GetTrip(j).destinationID - 1;
            distance[startID][destinationID] = tripWeight;
            (*shortestPathSequenceTable).at(startID).at(destinationID) = destinationID + 1;
        }
    }

    //Floyd Warshall Algorithm
    for(int k = 0; k < stationCount; k++)
    {
        for(int i = 0; i < stationCount; i++)
        {
            for(int j = 0; j < stationCount; j++)
            {
                if(distance[i][k] != INF && distance[k][j] != INF &&
                    distance[i][k] + distance[k][j] < distance[i][j])
                    {
                        distance[i][j] = distance[i][k] + distance[k][j];
                        // Update shortest path table to reflect new shorter node.
                        (*shortestPathSequenceTable).at(i).at(j) = (*shortestPathSequenceTable).at(i).at(k);
                    }
            }
        }
    }

}

int StationGraph::GetVertexCount()
{
    return stationCount;
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

bool StationGraph::PathExists(int startStationID, int targetStationID)
{
    Station startStation = GetStationFromDepartGraph(startStationID);
    Station targetStation = GetStationFromDepartGraph(targetStationID);

    if(!startStation.StationIsValid() || !targetStation.StationIsValid())
    {
        std::cout << "Invalid station, please try again.\n";
        return false;
    }
    std::vector<bool> discoveredSet(stationCount, false);
    std::queue<Station> frontiereQueue;

    discoveredSet[startStation.GetID() - 1] = true;
    frontiereQueue.push(startStation);

    while(!frontiereQueue.empty())
    {
        Station currentStation = frontiereQueue.front();
        frontiereQueue.pop();

        for(int i = 0; i < currentStation.GetTripCount(); i++)
        {
            // Store current adjacent node for processing.
            Station adjacentStation = GetStationFromDepartGraph(currentStation.GetTrip(i).destinationID);

            // if there is a connection between targetStation and any adjacent node, a path exists from start to target
            if(adjacentStation.GetID() == targetStation.GetID())
            {
                return true;
            }

            // If current adjacent node hasn't already been discovered, add it to frontiere queue and discover it.
            if(!discoveredSet[adjacentStation.GetID() - 1])
            {
                frontiereQueue.push(adjacentStation);
                discoveredSet[adjacentStation.GetID() - 1] = true;
            }
        }
    }
    // Frontiere queue empties before target is reached, no path exists.
    return false;
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



