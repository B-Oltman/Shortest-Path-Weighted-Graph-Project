#pragma once
#include <cstddef>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "trip.hpp"
#include "utility.hpp"
#include "route.hpp"
#include "station_graph.hpp"

class Schedule{
    public:
        //Constructor - create new schedule from data files.
        Schedule(std::string stationData, std::string trainsData);
        //Destructor - destroy schedule
        ~Schedule();
        //Print schedule for all stations
        void PrintCompleteSchedule();
        //Print schedule for selected station no arguments is overloaded to prompt for input
        void PrintStationSchedule();
        // int argument does not prompt user, iD must be passed in.
        void PrintStationSchedule(int stationID);
        //Print station number for given station name
        void LookUpStationId();
        //Print station name for given station number
        void LookUpStationName();
        std::string SimpleStationNameLookup(int stationID);
        //Returns whether there is a direct route from station A to station B
        void GetDirectRoute();
        //Returns whether there is any route from station A to station B
        void GetRoute();
        //Gets the shortest time and itinerary to go from A to B, paths are weighted by travel time only
        void ShortestTripLengthRideTime();
        //Gets the shortest time and itinerary to go from A to B, paths are weighted by layover time + travel time
        void ShortestTripLengthWithLayover();
        //Returns the shortest time and itinerary  to go from A to B when departing at a specific time only.
        void ShortestTripDepartureTime(); 
    private:
        std::vector<std::vector<std::string>> stationLookupTable;
        std::vector<std::vector<std::string>> tripDataTable;
        StationGraph* stationGraph;
        // Builds a lookup table to map station id to station name.
        void build_station_lookup_table(std::string stationData);        
        void build_trip_data_table(std::string trainsData);
        int prompt_twenty_four_time() const;
        int prompt_station_id() const;
        std::pair<int, int> prompt_station_pair_id() const;        
};

Schedule::Schedule(std::string stationData, std::string trainsData)
{
    build_station_lookup_table(stationData);
    build_trip_data_table(trainsData);
    stationGraph = new StationGraph(tripDataTable, stationLookupTable, stationLookupTable.size());
}

Schedule::~Schedule()
{
    if(stationGraph)
    {
        delete stationGraph;
    }
}

void Schedule::PrintCompleteSchedule()
{
    std::cout <<"                  TRAIN SCHEDULE\n";
    for(int i = 0; i < stationLookupTable.size(); i++)
    {
        std::cout << "***************************************************\n";
        PrintStationSchedule(i + 1);
    }
    std::cout << "******************End of Schedule******************\n";
    
}

void Schedule::PrintStationSchedule()
{
    int stationID = prompt_station_id();
    Station station = stationGraph->GetStationFromGraph(stationID);
    std::cout << "Schedule for " << SimpleStationNameLookup(station.GetID()) << std::endl;

    if (station.GetTripCount() == 0)
    {
        std::cout << "There are no scheduled departures for " << SimpleStationNameLookup(station.GetID()) << std::endl;
    }
    else
    {
        for (int i = 0; i < station.GetTripCount(); i++)
        {
            int destinationID = station.GetTrip(i).destinationID;
            int departureTime = station.GetTrip(i).departureTime;
            int arrivalTime = station.GetTrip(i).arrivalTime;
            std::cout << "Departure to " << SimpleStationNameLookup(destinationID) << " at "
                       << std::setw(4) << std::setfill('0') << departureTime << ", arriving at "
                        << std::setw(4) << std::setfill('0') << arrivalTime << std::endl;
        }
    }

    station = stationGraph->GetStationFromArrivalGraph(stationID);
    if (station.GetTripCount() == 0)
    {
        std::cout << "There are no scheduled arrivals for " << SimpleStationNameLookup(station.GetID()) << std::endl;
    }
    else
    {
        for (int i = 0; i < station.GetTripCount(); i++)
        {
            int departureID = station.GetTrip(i).destinationID;
            int arrivalTime = station.GetTrip(i).departureTime;
            std::cout << "Arrival from " << SimpleStationNameLookup(departureID) << " at "
                       << std::setw(4) << std::setfill('0') << arrivalTime << std::endl;
        }
    }
}

void Schedule::PrintStationSchedule(int stationID)
{
    Station station = stationGraph->GetStationFromGraph(stationID);

    if (station.StationIsValid())
    {
        std::cout << "Schedule for " << SimpleStationNameLookup(station.GetID()) << std::endl;
        if (station.GetTripCount() != 0)
        {        
            for (int i = 0; i < station.GetTripCount(); i++)
            {
                int destinationID = station.GetTrip(i).destinationID;
                int departureTime = station.GetTrip(i).departureTime;
                int arrivalTime = station.GetTrip(i).arrivalTime;
                std::cout << "Departure to " << SimpleStationNameLookup(destinationID) << " at "
                           << std::setw(4) << std::setfill('0') << departureTime << ", arriving at "  << std::setw(4) << std::setfill('0') 
                           << arrivalTime << std::endl;
            }
        }
        else
        {
            std::cout << "There are no trains leaving from "
                << SimpleStationNameLookup(station.GetID()) << std::endl;
        }

        station = stationGraph->GetStationFromArrivalGraph(stationID);
        if (station.GetTripCount() != 0)
        {
            for (int i = 0; i < station.GetTripCount(); i++)
            {
                int departureID = station.GetTrip(i).destinationID;
                int arrivalTime = station.GetTrip(i).departureTime;
                std::cout << "Arrival from " << SimpleStationNameLookup(departureID) << " at "
                           << std::setw(4) << std::setfill('0') << arrivalTime << std::endl;
            }
        }
        else
        {
            std::cout << "There are no scheduled arrivals for "
                << SimpleStationNameLookup(station.GetID()) << std::endl;
        }
    }
    else
    {
        std::cout << "There was a problem with the input\nin PrintStationSchedule, please try again.\n";
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
}

void Schedule::LookUpStationName()
{
    int stationID;
    std::cout << "Enter station id: ";
    std::cin >> stationID;
    //Clear input buffer
    Utility::ClearInStream();

    if(stationID > 0 && stationID <= stationLookupTable.size())
    {
        //stationID - 1 maps the input to the corresponding vector entry.
        std::cout << "Station " << stationLookupTable[stationID - 1][0] << " is " << 
            SimpleStationNameLookup(stationID) << std::endl;
    }
    else
    {
        std::cout <<"Invalid station id (enter value betweeen 1 and " << stationLookupTable.size() <<")\n";
    }
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

void Schedule::GetDirectRoute()
{
    std::pair<int, int> stationPair = prompt_station_pair_id();

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

void Schedule::GetRoute()
{
    std::pair<int, int> stationPair = prompt_station_pair_id();

    if(stationGraph->PathExists(stationPair.first, stationPair.second))
    {

        std::cout << "Service is available from " << SimpleStationNameLookup(stationPair.first) << 
        " to " << SimpleStationNameLookup(stationPair.second) << std::endl;
    }
    else
    {
        std::cout << "Service is NOT available from " << SimpleStationNameLookup(stationPair.first) << 
        " to " << SimpleStationNameLookup(stationPair.second) << std::endl;
    }
}

void Schedule::ShortestTripLengthRideTime()
{
    std::pair<int, int> stationPair = prompt_station_pair_id();
    Route tripRoute = stationGraph->GetShortestRoute(stationPair.first, stationPair.second, false);

    if (tripRoute.RouteIsValid())
    {
        int totalTripMins = 0;
        for (TripPlusLayover trip : tripRoute.tripList)
        {

            totalTripMins += trip.rideTimeToDestinationMins;
        }

        std::cout << "\nMinimum time spent on train from " << SimpleStationNameLookup(stationPair.first)
            << " to " << SimpleStationNameLookup(stationPair.second) << "\nis "
            << totalTripMins / 60 << " hours and " << totalTripMins % 60
            << " minutes. Layover time not included.\nItinerary\n----------\n";

        Departure startDeparture = tripRoute.departingStation;        
        for(int i = 0; i < tripRoute.tripList.size(); i++)
        {
            TripPlusLayover currentTrip = tripRoute.tripList[i];
            Departure endDeparture = stationGraph->GetDepartureFromGraph(currentTrip.destinationKey);

            std::cout << "Leave from " << SimpleStationNameLookup(startDeparture.GetStationID())
                << " at "  << std::setw(4) << std::setfill('0') << startDeparture.GetDepartureTime()
                << ", arrive at " << SimpleStationNameLookup(endDeparture.GetStationID()) << " at "
                 << std::setw(4) << std::setfill('0') << startDeparture.GetDepartureTime() + currentTrip.rideTimeToDestinationMins << std::endl;

            startDeparture = endDeparture;
        }
    }
    else
    {
        std::cout << "There is no route from " << SimpleStationNameLookup(stationPair.first)
            << " to " << SimpleStationNameLookup(stationPair.second) << ".\n";
    }
}

void Schedule::ShortestTripLengthWithLayover()
{
    std::pair<int, int> stationPair = prompt_station_pair_id();
    Route tripRoute = stationGraph->GetShortestRoute(stationPair.first, stationPair.second, true);

    if(tripRoute.RouteIsValid())
    {        
        int totalTripMins = 0;
        for(TripPlusLayover trip : tripRoute.tripList)
        {            
            totalTripMins += trip.tripWeight;
        }

        std::cout << "\nShortest overall travel time from " << SimpleStationNameLookup(stationPair.first)
            << " to " << SimpleStationNameLookup(stationPair.second) << " \nis " 
            << totalTripMins / 60 << " hours and " << totalTripMins % 60
            << " minutes including layovers.\nItinerary\n----------\n";

        Departure startDeparture = tripRoute.departingStation;
        for (int i = 0; i < tripRoute.tripList.size(); i++)
        {
            TripPlusLayover currentTrip = tripRoute.tripList[i];
            Departure endDeparture = stationGraph->GetDepartureFromGraph(currentTrip.destinationKey);

            std::cout << "Leave from " << SimpleStationNameLookup(startDeparture.GetStationID())
                      << " at "  << std::setw(4) << std::setfill('0') << startDeparture.GetDepartureTime()
                      << ", arrive at " << SimpleStationNameLookup(endDeparture.GetStationID()) << " at "
                       << std::setw(4) << std::setfill('0') << startDeparture.GetDepartureTime() + currentTrip.rideTimeToDestinationMins
                       << std::endl;

            startDeparture = endDeparture;
        }
    }
    else
    {
        std::cout << "There is no route from " << SimpleStationNameLookup(stationPair.first) << " to "
        << SimpleStationNameLookup(stationPair.second) << ".\n";
    }
}

void Schedule::ShortestTripDepartureTime()
{
    std::pair<int, int> stationPair = prompt_station_pair_id();
    std::cout << "When would you like to leave?\n";

    int time = prompt_twenty_four_time();
    Route tripRoute = stationGraph->GetRouteFromTime(time, stationPair.first, stationPair.second);
    if (tripRoute.RouteIsValid())
    {
        int totalTripMins = 0;
        for (TripPlusLayover trip : tripRoute.tripList)
        {
            totalTripMins += trip.tripWeight;
        }

        std::cout << "\nShortest overall travel time from " << SimpleStationNameLookup(stationPair.first)
                  << " to " << SimpleStationNameLookup(stationPair.second) << " \nis "
                  << totalTripMins / 60 << " hours and " << totalTripMins % 60
                  << " minutes including layovers.\nItinerary\n----------\n";

        Departure startDeparture = tripRoute.departingStation;
        for (int i = 0; i < tripRoute.tripList.size(); i++)
        {
            TripPlusLayover currentTrip = tripRoute.tripList[i];
            Departure endDeparture = stationGraph->GetDepartureFromGraph(currentTrip.destinationKey);

            std::cout << "Leave from " << SimpleStationNameLookup(startDeparture.GetStationID())
                      << " at " << std::setw(4) << std::setfill('0') << startDeparture.GetDepartureTime()
                      << ", arrive at " << SimpleStationNameLookup(endDeparture.GetStationID()) << " at " 
                      << std::setw(4) << std::setfill('0')  << startDeparture.GetDepartureTime() + currentTrip.rideTimeToDestinationMins
                      << std::endl;

            startDeparture = endDeparture;
        }
    }
    else
    {
        std::cout << "There are no routes from " << SimpleStationNameLookup(stationPair.first) << " to "
                  << SimpleStationNameLookup(stationPair.second) << " leaving at "  << std::setw(4) << std::setfill('0') << time;

        if(time > 1300)
        {
            std::cout << " or " << std::setw(4) << std::setfill('0') << time - 1200 << std::endl;
        }
        else
        {
            std::cout << std::endl;
        }
        
    }
}

void Schedule::build_station_lookup_table(std::string stationData)
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

    // sort the data in station table, not guaranteed to come in sorted.
   std::sort(stationLookupTable.begin(), stationLookupTable.end(),
        [](const std::vector<std::string>& a, const std::vector<std::string>& b) { return a[0] < b[0]; });
}

void Schedule::build_trip_data_table(std::string trainsData)
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

int Schedule::prompt_twenty_four_time() const
{
    std::cout << "Enter time (HH:MM): ";
    std::pair<int, int> time = {-1,-1};

    int firstH = 0;
    int secondH = 0;
    int firstM = 0;
    int secondM = 0;   

    Utility::ClearInStream(); 
    bool valid = false;
    while (!valid)
    {            
        std::string line;
        std::getline(std::cin, line);        

        firstH = line[0] - '0';
        secondH = line[1] - '0';        
        firstM = line[3] - '0';
        secondM = line[4] - '0';        

        if((firstH == 0 && (secondH > 0 && secondH <= 9)) ||
            (firstH == 1 && (secondH >= 0 && secondH <= 2)) &&
            (firstM >= 0 && firstM < 6) &&
            (secondM >= 0 && secondM <= 9))
        {
            valid = true;
        }
        else
        {
            valid = false;
            std::cout << "Invalid time, must be in HH:MM format: ";            
        }
    }

    int twentyFourTime = 0;
    int hour = (firstH * 10) + secondH;
    int min = (firstM * 10) + secondM;
    if(hour < 12 && hour > 1)
    {
        twentyFourTime = (hour + 12) * 100;
    }
    else
    {
        twentyFourTime = hour * 100;
    }

    return (twentyFourTime += min);    
}

int Schedule::prompt_station_id() const
{
    std::cout << "Enter station id: ";
    int stationID = Utility::GetIntFromUser();
    while(stationGraph->GetStationFromGraph(stationID).StationIsValid() == false)
    {
        std::cout << "Station id invalid, try again: ";
        stationID = Utility::GetIntFromUser();
    }

    return stationID;
}

std::pair<int, int> Schedule::prompt_station_pair_id() const
{  
    std::cout << "Enter departure station id: ";
    int departID = Utility::GetIntFromUser();
    while(stationGraph->GetStationFromGraph(departID).StationIsValid() == false)
    {
        std::cout << "Departure station id invalid, try again: ";
        departID = Utility::GetIntFromUser();
    }

    std::cout << "Enter destination station id: ";
    int destID = Utility::GetIntFromUser();
    while(stationGraph->GetStationFromGraph(destID).StationIsValid() == false)
    {
        std::cout << "Destination station id invalid, try again: ";
        destID = Utility::GetIntFromUser();
    }

    return {departID, destID};
}



