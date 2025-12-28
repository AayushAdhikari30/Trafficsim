#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>

class Vehicle {
    private:
        std::string licensePlate;
       char laneName;
       int laneNumber;
        std::time_t entryTime;
    public:
    Vehicle(std::string plate,char lane,int laneNum );

    std::string getLicensePlate() const;
    char getLaneName() const;
    int getLaneNumber() const;
    std::time_t getEntryTime() const;

    void display() const;
};
#endif 