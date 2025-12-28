#include "Vehicle.h"
#include <iostream>


Vehicle::Vehicle(std::string plate, char road, int lane)
{
    this->licensePlate = plate;
    this->roadName = road;
    this->laneNumber = lane;
    this->entryTime = std::time(nullptr);
}


std::string Vehicle::getLicensePlate() const
{
    return licensePlate;
}


char Vehicle::getRoadName() const
{
    return roadName;
}


int Vehicle::getLaneNumber() const
{
    return laneNumber;
}


std::time_t Vehicle::getEntryTime() const
{
    return entryTime;
}


char Vehicle::getLaneName() const
{
    return roadName;  
}


void Vehicle::display() const 
{
    std::cout << "License Plate: " << licensePlate 
              << ", Road: " << roadName 
              << ", Lane: " << laneNumber 
              << ", Entry Time: " << entryTime << std::endl;
}