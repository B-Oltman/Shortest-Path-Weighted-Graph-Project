#pragma once
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include "station.hpp"
#include "departure.hpp"

class StationGraph{
    public:
        StationGraph(std::vector<std::vector<std::string>> const tripData, int stationsCount);
        ~StationGraph();
        bool DirectPathExists(int station1ID, int station2ID);
        bool PathExists(int startStationID, int targetStationID);
        Station GetStationFromGraph(int stationID);
        Station GetStationFromArrivalGraph(int stationID);
        std::vector<Trip> GetShortestRoute(int departureID, int destinationID);
        int GetVertexCount();
        void DebugTestPrintShortPaths();
    private:
        const int stationCount;
        //Departure graph is the standard graph used for operations concerned with going from a starting node to a
        // destination node, this is the majority of the functions in the program.
        std::vector<Station>* stationsGraphList;
        // Arrivals graph is an inverted version of the graph so that arrivals to a station can be
        // looked up easily. Only used for a few functions in the program.
        std::vector<Station>* stationArrivalsGraphList;
        // Used to calculate shortest route accounting for layovers.
        std::vector<Departure>* departureGraphList;
        std::vector<std::vector<int>>* shortestPathSequenceTable;
        std::vector<std::vector<int>>* shortestPathWithLayoverSequenceTable;
        void floyd_warshal_shortest_paths();
        void floyd_warshal_shortest_paths_include_layovers();
        void build_stations_graph(std::vector<std::vector<std::string>> tripData);
        void build_station_arrivals_graph(std::vector<std::vector<std::string>> tripData);
        void build_departures_graph(std::vector<std::vector<std::string>> tripData);
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
    build_stations_graph(tripDataTable);
    build_station_arrivals_graph(tripDataTable);
    build_departures_graph(tripDataTable);
    floyd_warshal_shortest_paths();
    floyd_warshal_shortest_paths_include_layovers();
}

StationGraph::~StationGraph()
{
    if(stationsGraphList) delete stationsGraphList;
    if(stationArrivalsGraphList) delete stationArrivalsGraphList;
    if(departureGraphList) delete departureGraphList;
    if(shortestPathSequenceTable) delete shortestPathSequenceTable;
    if(shortestPathWithLayoverSequenceTable) delete shortestPathWithLayoverSequenceTable;
}

void StationGraph::build_stations_graph(std::vector<std::vector<std::string>> tripDataTable)
{
    // Use a temporary table to hold all trips so that they
    // can be passed into station constructor.
    // We don't want to allow the addition of new graph elements after construction because we are pre-computing shortest paths
    // so not making any public functions to add nodes or edges.
    stationsGraphList = new std::vector<Station>;

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
        stationsGraphList->push_back({i + 1, tempTripTable[i]});
    }
}

void StationGraph::build_departures_graph(std::vector<std::vector<std::string>> tripDataTable)
{
    departureGraphList = new std::vector<Departure>;
    std::vector<std::pair<std::pair<int, int>, std::vector<TripPlusLayover>>> tempTripTable;

    for(int i = 0; i < tripDataTable.size(); i++)
    {
        tempTripTable.push_back({});
    }

    for(int i = 0; i < tripDataTable.size(); i++)
    {
        for(int j = 0; j < tripDataTable.size(); j++)
        {
            // skip current index
            if(j == i) continue;

            if(tripDataTable[i][0] == tripDataTable[j][0])
            {
                if(tripDataTable[i][3] < tripDataTable[j][2])
                {
                    int destinationID = stoi(tripDataTable[j][1]);
                    int departureTime = stoi(tripDataTable[j][2]);
                    int rideTimeToDestination = stoi(tripDataTable[i][3]) - stoi(tripDataTable[i][2]);
                    int layoverAtDestination = stoi(tripDataTable[j][3]) - stoi(tripDataTable[i][3]);
                    int totalTripTime = rideTimeToDestination + layoverAtDestination;
                    
                    // Search adjacency list for matching vertex to insert new edge data. Can't be a simple index with current setup.
                    // May be wise to replace this with a sorting and binary search eventually.
                    for(int k = 0; k < tripDataTable.size(); k++)
                    {
                        //If the target edge departure time and departure station match, this is the correct insertion point, add edge to adjacency list.
                        if(stoi(tripDataTable[k][0]) == stoi(tripDataTable[j][0]) && stoi(tripDataTable[k][2]) == stoi(tripDataTable[j][2]))
                        {
                            tempTripTable[k].first.first = departureTime;
                            tempTripTable[k].first.second = stoi(tripDataTable[j][0]);
                            tempTripTable[k].second.push_back({destinationID, rideTimeToDestination, layoverAtDestination, totalTripTime});
                        }
                    }
                }
            }
        }
    }

    // Populte the departure graph using data from tempTripTable.
    for(int i = 0; i < tempTripTable.size(); i++)
    {
        departureGraphList->push_back({tempTripTable[i].second, i, tempTripTable[i].first.second, tempTripTable[i].first.first});
    } 
}

void StationGraph::build_station_arrivals_graph(std::vector<std::vector<std::string>> tripDataTable)
{
    stationArrivalsGraphList = new std::vector<Station>;

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
        stationArrivalsGraphList->push_back({i + 1, tempTripTable[i]});
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
        Station currentStation = (*stationsGraphList)[i];
        for(int j = 0; j < currentStation.GetTripCount(); j++)
        {
            int tripWeight = currentStation.GetTrip(j).travelTimeMins;
            int startID = i;
            int destinationID = currentStation.GetTrip(j).destinationID - 1;            

            distance[startID][destinationID] = tripWeight;
            (*shortestPathSequenceTable).at(startID).at(destinationID) = destinationID + 1;
        }
    }

    //Floyd Warshal Algorithm
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

void StationGraph::floyd_warshal_shortest_paths_include_layovers()
{
}

std::vector<Trip> StationGraph::GetShortestRoute(int departureID, int destinationID)
{
    std::vector<Trip> shortPath;
    
    int nextStopID = departureID;
    bool endOfPath = false;

    while(!endOfPath)
    {                     
        Station currentNode = GetStationFromGraph(nextStopID);
        if(currentNode.StationIsValid())
        {            
            nextStopID = (*shortestPathSequenceTable).at(nextStopID - 1).at(destinationID - 1);            
            if(currentNode.GetID() == destinationID || nextStopID == 0)
            {                
                endOfPath = true;
            }
            else
            {                               
                Trip nextTrip = currentNode.FindTripByDestination(nextStopID);                
                shortPath.push_back(nextTrip);       
            }                       
        }
        else
        {
            return {};
        }        
    }

    return shortPath;
}

int StationGraph::GetVertexCount()
{
    return stationCount;
}

Station StationGraph::GetStationFromGraph(int stationID)
{
    int iDAsZeroIndex = stationID - 1;
    if(iDAsZeroIndex < stationsGraphList->size() && iDAsZeroIndex >= 0)
    {
        return (*stationsGraphList)[iDAsZeroIndex];
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
    if(iDAsZeroIndex < stationArrivalsGraphList->size() && iDAsZeroIndex >= 0)
    {
        return (*stationArrivalsGraphList)[iDAsZeroIndex];
    }
    else
    {
        // return invalid station if bad station ID
        return Station({-1, {}});
    } 
}

bool StationGraph::PathExists(int startStationID, int targetStationID)
{
    Station startStation = GetStationFromGraph(startStationID);
    Station targetStation = GetStationFromGraph(targetStationID);

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
            Station adjacentStation = GetStationFromGraph(currentStation.GetTrip(i).destinationID);

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
    Station currentStation = GetStationFromGraph(station1ID);
    for(int i = 0; i < currentStation.GetTripCount(); i++)
    {
        if(currentStation.GetTrip(i).destinationID == station2ID)
        {
            return true;
        }
    }
    return false;
}



