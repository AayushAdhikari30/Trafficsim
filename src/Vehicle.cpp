#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle(std::string plate,char lane)
{
    this ->licensePlate=plate;
    this->laneName=lane;
    this->entryTime=std::time(nullptr);
}

std::string Vehicle::getLicensePlate() const
{
    return licensePlate;
}

char Vehicle::getLaneName() const
{
    return laneName;
}

std::time_t Vehicle::getEntryTime() const
{
    return entryTime;
}

void Vehicle::display() const 
{
    std::cout << "License Plate: " << licensePlate << ",Lane: " << laneName << ",Entry Time: " <<
    entryTime << std::endl;
}