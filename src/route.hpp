#pragma once
#include <vector>
#include "departure.hpp"
#include "trip.hpp"

struct Route {
    Departure departingStation;
    std::vector<TripPlusLayover> tripList;
    Departure destinationStation;
};
