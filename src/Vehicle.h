#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>

class Vehicle {
private:
    std::string licensePlate;
    char roadName;      
    int laneNumber;     
    std::time_t entryTime;
    
public:
   
    Vehicle(std::string plate, char road, int lane);
    
   
    std::string getLicensePlate() const;
    char getRoadName() const;         
    int getLaneNumber() const;
    std::time_t getEntryTime() const;
    
  
    void display() const;
    
   
    char getLaneName() const;  
};

#endif