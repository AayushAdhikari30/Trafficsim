#include "TrafficGenerator.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>

TrafficGenerator::TrafficGenerator():
    randomEngine(std::time(nullptr)),
    roadDistribution(0, 99),     
    laneDistribution(1, 3),      
    timeDistribution(0.5, 2.0)    
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

char TrafficGenerator::selectRandomRoad() {
    int random = roadDistribution(randomEngine);
    
    if (random < 40)       
        return 'A';
    else if (random < 60)   
        return 'B';
    else if (random < 80)   
        return 'C';
    else                    
        return 'D';
}

int TrafficGenerator::selectRandomLane() {
    int random = randomEngine() % 100;
    
    // Lane distribution:
    // Lane 1 (Right turn - FREE): 20%
    // Lane 2 (Straight - CONTROLLED): 60%
    // Lane 3 (Left turn - FREE): 20%
    
    if (random < 20)        // 0-19: Lane 1 (Right turn)
        return 1;
    else if (random < 80)   // 20-79: Lane 2 (Straight)
        return 2;
    else                    // 80-99: Lane 3 (Left turn)
        return 3;
}

bool TrafficGenerator::writeVehicleToFile(const std::string& plate, char road, int lane) {
    std::string filename;
    
    switch(road) {
        case 'A': filename = "lane_A.txt"; break;
        case 'B': filename = "lane_B.txt"; break;
        case 'C': filename = "lane_C.txt"; break;
        case 'D': filename = "lane_D.txt"; break;
        default: return false;
    }
    
    std::ofstream file(filename, std::ios::app);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    file << plate << "," << road << "," << lane << std::endl;
    file.close();
    
    return true;
}

void TrafficGenerator::generateSingleVehicle() {
    std::string plate = generateLicensePlate();
    char road = selectRandomRoad();
    int lane = selectRandomLane();
    
    if (writeVehicleToFile(plate, road, lane)) {
        std::cout << "Generated: " << plate 
                  << " -> Road " << road 
                  << " Lane " << lane;
        
        if (lane == 1) {
            std::cout << " [🟢 FREE RIGHT]";
        } else if (lane == 2) {
            std::cout << " [🔴 CONTROLLED]";
        } else if (lane == 3) {
            std::cout << " [🟢 FREE LEFT]";
        }
        std::cout << std::endl;
    }
    else {
        std::cerr << "Failed to generate vehicle" << std::endl;
    }
}

void TrafficGenerator::run() {
    int vehicleCount = 0;
    
    std::cout << "\n╔════════════════════════════════════════════╗" << std::endl;
    std::cout << "║   Traffic Generator Started (3-Lane)      ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════╝" << std::endl;
    std::cout << "\n📊 Traffic Distribution:" << std::endl;
    std::cout << "   Road A (Priority): 40% of traffic" << std::endl;
    std::cout << "   Roads B, C, D:     20% each" << std::endl;
    std::cout << "\n🛣️  Lane Distribution:" << std::endl;
    std::cout << "   Lane 1 (Right):  20% - 🟢 FREE FLOW" << std::endl;
    std::cout << "   Lane 2 (Straight): 60% - 🔴 CONTROLLED" << std::endl;
    std::cout << "   Lane 3 (Left):   20% - 🟢 FREE FLOW" << std::endl;
    std::cout << "\nPress Ctrl+C to stop\n" << std::endl;
    
    while (true) {
        double waitTime = timeDistribution(randomEngine);
        Sleep(static_cast<int>(waitTime * 1000));
        
        std::string plate = generateLicensePlate();
        char road = selectRandomRoad();
        int lane = selectRandomLane();
        
        if (writeVehicleToFile(plate, road, lane)) {
            vehicleCount++;
            std::cout << "[" << vehicleCount << "] " << plate 
                      << " → Road " << road << "L" << lane;
            
            if (lane == 1) {
                std::cout << " 🟢R";  // Right turn
            } else if (lane == 2) {
                std::cout << " 🔴S";  // Straight (controlled)
            } else {
                std::cout << " 🟢L";  // Left turn
            }
            std::cout << std::endl;
        }
        else {
            std::cerr << "Error writing vehicle to file!" << std::endl;
        }
    }
}