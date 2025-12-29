#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>
#include <cmath>

class Vehicle {
private:
    std::string licensePlate;
    char roadName;      
    int laneNumber;     
    std::time_t entryTime;
    
   
    float x, y;              
    float speed;           
    bool isMoving;           
    float targetX, targetY;  
    
public:
    Vehicle(std::string plate, char road, int lane);
    
    std::string getLicensePlate() const;
    char getRoadName() const;
    int getLaneNumber() const;
    std::time_t getEntryTime() const;
    char getLaneName() const;
    
    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }
    void setTarget(float tx, float ty) { targetX = tx; targetY = ty; }
    void setMoving(bool moving) { isMoving = moving; }
    bool getIsMoving() const { return isMoving; }
    float getSpeed() const { return speed; }
    void setSpeed(float s) { speed = s; }
    
    void updatePosition(float deltaTime);
    bool hasReachedTarget() const;
    
    void display() const;
};

#endif