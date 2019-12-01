#pragma once
#include <vector>
#include "trip.hpp"

class Station {
    public:
        int GetID();
        Station(int ID, std::vector<Trip>& tripArray);
    private:
        std::vector<Trip> trips;
        int stationID;
};

Station::Station(int ID, std::vector<Trip>& tripArray)
{
    stationID = ID;
    trips = tripArray;
}

int Station::GetID()
{
    return stationID;
}