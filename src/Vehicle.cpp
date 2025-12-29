#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle(std::string plate, char road, int lane)
    : licensePlate(plate), roadName(road), laneNumber(lane),
      x(0), y(0), targetX(0), targetY(0),
      isMoving(false), speed(100.0f),
      turnStage(0)
{
    this->entryTime = std::time(nullptr);
}



int Vehicle::getTurnStage() const {
    return turnStage;
}

void Vehicle::setTurnStage(int stage) {
    turnStage = stage;
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

void Vehicle::updatePosition(float deltaTime)
{
    if (!isMoving) return;

    float dx = targetX - x;
    float dy = targetY - y;

    float distance = std::sqrt(dx * dx + dy * dy);

    // Increased threshold from 1.0f to 2.0f for better stability
    if (distance < 2.0f) {
        x = targetX;
        y = targetY;
        isMoving = false;
        return;
    }

    // Calculate movement
    float moveDistance = speed * deltaTime;
    
    // Prevent overshooting
    if (moveDistance > distance) {
        moveDistance = distance;
    }

    float dirX = dx / distance;
    float dirY = dy / distance;

    x += dirX * moveDistance;
    y += dirY * moveDistance;
}

bool Vehicle::hasReachedTarget() const
{
    float dx = targetX - x;
    float dy = targetY - y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < 2.0f;
}

void Vehicle::display() const 
{
    std::cout << "License Plate: " << licensePlate 
              << ", Road: " << roadName 
              << ", Lane: " << laneNumber 
              << ", Entry Time: " << entryTime << std::endl;
}