#ifndef TRAFFICGENERATOR_H
#define TRAFFICGENERATOR_H

#include <string>
#include <random>

class TrafficGenerator {
private:
    std::mt19937 randomEngine;
    std::uniform_int_distribution<int> roadDistribution;
    std::uniform_int_distribution<int> laneDistribution;
    std::uniform_real_distribution<double> timeDistribution;
    
    std::string generateLicensePlate();
    char selectRandomRoad();
    int selectRandomLane();
    bool writeVehicleToFile(const std::string& plate, char road, int lane);
    
public:
    TrafficGenerator();
    void generateSingleVehicle();
    void run();
};

#endif