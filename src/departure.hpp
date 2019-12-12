#pragma once
#include <vector>
#include "trip.hpp"

class Departure {
    public:
        int GetStationID() const;
        int GetTripCount() const;
        int GetLookUpKey() const;
        bool IsFinalDestination();
        TripPlusLayover GetTrip(int tripIndex) const;
        TripPlusLayover FindTripByDestinationKey(int destinationKey);
        Departure(std::vector<TripPlusLayover> tripArray, int ID, int key, int departure);
    private:
        std::vector<TripPlusLayover> validTrips;
        int lookUpKey;
        int stationID;
        int departureTime;
};

Departure::Departure(std::vector<TripPlusLayover> tripArray, int ID, int key, int departure)
{
    validTrips = tripArray;
    stationID = ID;
    lookUpKey = key;
    departureTime = departure;
}

bool Departure::IsFinalDestination()
{
    return validTrips.size() == 0;
}

int Departure::GetLookUpKey() const
{
    return lookUpKey;
}

TripPlusLayover Departure::FindTripByDestinationKey(int destinationKey)
{
    for(TripPlusLayover trip : validTrips)
    {
        if(trip.destinationKey == destinationKey)
        {
            return trip;
        }
    }

    return {-1};
}

TripPlusLayover Departure::GetTrip(int tripIndex) const
{
    return validTrips[tripIndex];
}

int Departure::GetTripCount() const
{
    return validTrips.size();
}

int Departure::GetStationID() const
{
    return stationID;
}