#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle(std::string plate,char lane,int laneNum )
{
    this ->licensePlate=plate;
    this->laneName=lane;
    this->laneNumber=laneNum;
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
int Vehicle::getLaneNumber() const
{
    return laneNumber;
}
std::time_t Vehicle::getEntryTime() const
{
    return entryTime;
}

void Vehicle::display() const 
{
    std::cout << "License Plate: " << licensePlate << ",Lane: " << laneName << " " << laneNumber << ",Entry Time: " <<
    entryTime << std::endl;
}