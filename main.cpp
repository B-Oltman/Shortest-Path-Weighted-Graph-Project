#include <fstream>
#include <sstream>
#include <string>
#include <cstddef>
#include <iostream>
#include <vector>
#include "utility.hpp"
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

    bool quit = false;
    while(!quit)
    {
        std::cout << "========================================================================\n"
        << "READING RAILWAYS SCHEDULER\n"
        << "========================================================================\n"
        << "Options - (Enter number of your selected option)\n"
        << "(1) - Print full schedule\n"
        << "(2) - Print station schedule\n"
        << "(3) - Look up stationd id\n"
        << "(4) - Look up station name\n"
        << "(5) - Servie available\n"
        << "(6) - Nonstop service available\n"
        << "(7) - Find route (Shortest riding time)\n"
        << "(8) - Find route (Shortest overall travel time)\n"
        << "(9) - Find route (Shortest time, at specific departure time)\n"
        << "(0) - Exit\n";

        std::cout << "Enter choice: ";

        int choice = -1;
        std:: cin >> choice;
        if(std::cin.fail())
        {
            choice = -1;
            Utility::ClearInStream();
        }

        switch (choice)
        {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                trainSchedule.LookUpStationName();
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 0:
                quit = true;
                break;
            default:
                std::cout <<"Invalid choice (enter number 0-9).\n";
                break;    
        }
    }

}