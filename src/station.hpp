#pragma once
#include <vector>
#include "trip.hpp"

class Station {
    public:
        int GetID() const;
        int GetTripCount() const;
        Trip GetTrip(int tripIndex) const;
        Trip FindTripByDestination(int destinationID);
        bool StationIsValid() const;
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

bool Station::StationIsValid() const
{
    return (stationID > 0);
}

Trip Station::FindTripByDestination(int destinationID)
{
    for(Trip trip : trips)
    {
        if(trip.destinationID == destinationID)
        {
            return trip;
        }
    }

    return {-1};
}

Trip Station::GetTrip(int tripIndex) const
{
    return trips[tripIndex];
}

int Station::GetTripCount() const
{
    return trips.size();
}

int Station::GetID() const
{
    return stationID;
}