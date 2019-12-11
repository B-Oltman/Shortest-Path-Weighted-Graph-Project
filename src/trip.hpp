#pragma once

struct Trip {
    int destinationID;
    int departureTime;
    int arrivalTime;
    int travelTimeMins;
};

struct TripPlusLayover{
    int destinationKey;
    int rideTimeToDestinationMins;
    int layoverAtDestinationMins;
    int tripWeight;
};