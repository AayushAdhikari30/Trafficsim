#include "TrafficGenerator.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>  

TrafficGenerator::TrafficGenerator():
    randomEngine(std::time(nullptr)),
    laneDistribution(0,99),
    timeDistribution(0.5,2.0)
{}

std::string TrafficGenerator::generateLicensePlate() {
    const char* provinces[] = {"BA", "GA", "LU", "PR", "KA", "SU", "SE"};
    
    int provinceIndex = randomEngine() % 7;
    std::string plate = provinces[provinceIndex];
    
    plate += std::to_string((randomEngine() % 9) + 1);
    
    const char* letters[] = {"PA", "KA", "JA", "MA", "CHA", "TA", "NA", "XX", "YY", "ZZ"};
    int letterIndex = randomEngine() % 10;
    plate += letters[letterIndex];
    
    int number = randomEngine() % 10000;
    
    if (number < 10) {
        plate += "000" + std::to_string(number);
    }
    else if (number < 100) {
        plate += "00" + std::to_string(number);
    }
    else if (number < 1000) {
        plate += "0" + std::to_string(number);
    }
    else {
        plate += std::to_string(number);
    }
    
    return plate;
}

char TrafficGenerator::selectRandomLane() {
    int random = laneDistribution(randomEngine);

    if(random < 40)
        return 'A';
    else if (random < 60)
        return 'B';
    else if (random < 80)
        return 'C';
    else
        return 'D';
}

bool TrafficGenerator::writeVehicleToFile(const std::string& plate, char lane) {
    std::string filename;
    switch(lane) {
        case 'A': filename = "lane_A.txt"; break;
        case 'B': filename = "lane_B.txt"; break;
        case 'C': filename = "lane_C.txt"; break;
        case 'D': filename = "lane_D.txt"; break;
        default: return false;
    }
    
    std::ofstream file(filename, std::ios::app);

    if(!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    file << plate << "," << lane << std::endl;
    file.close();
    
    return true;
}

void TrafficGenerator::generateSingleVehicle() {
    std::string plate = generateLicensePlate();
    char lane = selectRandomLane();

    if(writeVehicleToFile(plate, lane)) {
        std::cout << "Generated: " << plate << " in lane " << lane << std::endl;
    }
    else {
        std::cerr << "Failed to generate vehicle" << std::endl;
    }
}

void TrafficGenerator::run() {
    int vehicleCount = 0;

    while(true) {
        double waitTime = timeDistribution(randomEngine);

        
        Sleep(static_cast<int>(waitTime * 1000));

        std::string plate = generateLicensePlate();
        char lane = selectRandomLane();

        if(writeVehicleToFile(plate, lane)) {
            vehicleCount++;
            std::cout << "[" << vehicleCount << "] Vehicle " << plate 
                      << " arrived at Lane " << lane 
                      << " (wait: " << waitTime << "s)" << std::endl;
        }
        else {
            std::cerr << "Error writing Vehicle to file!" << std::endl;
        }
    }
}
