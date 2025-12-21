#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <iostream>

using namespace std;

class TrafficLight {
private:
    char currentLane;
    char trafficLightColor; 
    bool priorityMode;
    int greenDuration;

public:
    TrafficLight();

    char getCurrentLane() const;
    bool isGreen(char lane) const;
    void switchtoNextLane();
    void activatePriorityMode();
    void deactivatePriorityMode();
    bool isPriorityMode() const;

    void display() const;
};

#endif