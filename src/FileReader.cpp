#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>


std::vector<Vehicle> FileReader::readLaneFile(const std::string& filename) {
    std::vector<Vehicle> vehicles;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return vehicles;  
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
       
        
        
        size_t firstComma = line.find(',');
        size_t secondComma = line.find(',', firstComma + 1);
        
        if (firstComma != std::string::npos && secondComma != std::string::npos) {
            
            std::string plate = line.substr(0, firstComma);
            char road = line[firstComma + 1];
            int lane = std::stoi(line.substr(secondComma + 1));
            
           
            vehicles.push_back(Vehicle(plate, road, lane));
        }
    }
    
    file.close();
    return vehicles;
}


void FileReader::clearFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);
    file.close();
}


void FileReader::readAllLaneFiles(
    std::vector<Vehicle>& lane_A,
    std::vector<Vehicle>& lane_B,
    std::vector<Vehicle>& lane_C,
    std::vector<Vehicle>& lane_D
) {
   
    lane_A = readLaneFile("lane_A.txt");
    lane_B = readLaneFile("lane_B.txt");
    lane_C = readLaneFile("lane_C.txt");
    lane_D = readLaneFile("lane_D.txt");
    
    
    if (!lane_A.empty()) clearFile("lane_A.txt");
    if (!lane_B.empty()) clearFile("lane_B.txt");
    if (!lane_C.empty()) clearFile("lane_C.txt");
    if (!lane_D.empty()) clearFile("lane_D.txt");
}