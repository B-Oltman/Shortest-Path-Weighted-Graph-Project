#pragma once
#include <vector>
#include "departure.hpp"
#include "trip.hpp"

struct Route {
    bool RouteIsValid();
    Departure departingStation;
    std::vector<TripPlusLayover> tripList;
};

bool Route::RouteIsValid()
{
    if(tripList.size() > 0 && departingStation.GetTripCount() > 0)
    {
        return true;
    }         
    else
    {
        return false;
    }
    
}