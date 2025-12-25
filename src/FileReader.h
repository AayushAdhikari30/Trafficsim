#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>
#include "Vehicle.h"

class FileReader {
public:
    
    static std::vector<Vehicle> readLaneFile(const std::string& filename);
    
  
    static void clearFile(const std::string& filename);
    
  
    static void readAllLaneFiles(
        std::vector<Vehicle>& lane_A,
        std::vector<Vehicle>& lane_B,
        std::vector<Vehicle>& lane_C,
        std::vector<Vehicle>& lane_D
    );
};

#endif