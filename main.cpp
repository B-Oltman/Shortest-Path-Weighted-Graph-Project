#include <fstream>
#include <sstream>
#include <string>
#include <cstddef>
#include <iostream>
#include <vector>
#include "schedule.hpp"

using namespace std;

int main(int argc, char** argv)
{
    std::ifstream stationFile;
    std::stringstream stationData;
    std::ifstream trainFile;
    std::stringstream trainData;

    if(argc < 3)
    {
        std::cout << "useage: ./sched.out <stations.dat> <trains.dat>\n";
        return 0;
    }

    // Get file data into a string so schedule can be constructed
    stationFile.open(argv[1]);
    stationData << stationFile.rdbuf();
    stationFile.close();

    stationFile.open(argv[2]);
    trainData << stationFile.rdbuf();
    trainFile.close();

    Schedule trainSchedule(stationData.str() , trainData.str());

}