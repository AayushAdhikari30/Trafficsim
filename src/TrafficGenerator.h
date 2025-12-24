#ifndef TRAFFICGENERATOR_H
#define TRAFFICGENERATOR_H

#include <string>
#include <fstream>
#include <random>

class TrafficGenerator {
    private:
        std::mt19937 randomEngine;
        std::uniform_int_distribution<int> laneDistribution;
        std::uniform_real_distribution<double> timeDistribution;
        std::string generateLicensePlate();
        
        char selectRandomLane();
        bool writeVehicleToFile(const std::string& plate,char lane);

    public:
        TrafficGenerator();

        void run();

        void generateSingleVehicle();

};

#endif