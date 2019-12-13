#pragma once

struct Trip {
    int destinationID;
    int departureTime;
    int arrivalTime;    
};

struct TripPlusLayover{
    int destinationKey;
    int rideTimeToDestinationMins;
    int layoverAtDestinationMins;
    int tripWeight;
};