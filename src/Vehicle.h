#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>

class Vehicle {
    private:
        std::string licensePlate;
       char laneName;
        std::time_t entryTime;
    public:
    Vehicle(std::string plate,char lane);

    std::string getLicensePlate() const;
    char getLaneName() const;
    std::time_t getEntryTime() const;

    void display() const;
};
#endif 