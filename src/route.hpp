#pragma once
#include <vector>
#include "departure.hpp"
#include "trip.hpp"

struct Route {
    bool RouteIsValid();
    Departure departingStation;
    std::vector<TripPlusLayover> tripList;
    Departure destinationStation;
};

bool Route::RouteIsValid()
{
    if(tripList.size() > 0 && departingStation.GetTripCount() > 0 && destinationStation.IsFinalDestination())
    {
        return true;
    }         
    else
    {
        return false;
    }
    
}