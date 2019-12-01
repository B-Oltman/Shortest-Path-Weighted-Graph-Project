#pragma once
#include <vector>
#include "trip.hpp"

class Station {
    public:
        int GetID();
        int GetTripCount();
        Trip GetTrip(int tripIndex);
        Station(int ID, std::vector<Trip> tripArray);
    private:
        std::vector<Trip> trips;
        int stationID;
};

Station::Station(int ID, std::vector<Trip> tripArray)
{
    trips = tripArray;
    stationID = ID;
}

Trip Station::GetTrip(int tripIndex)
{
    return trips[tripIndex];
}

int Station::GetTripCount()
{
    return trips.size();
}

int Station::GetID()
{
    return stationID;
}