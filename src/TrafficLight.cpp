#include "TrafficLight.h"

TrafficLight::TrafficLight() {
    currentLane = 'A';
    priorityMode = false;  
    greenDuration = 5;
}

char TrafficLight::getCurrentLane() const {
    return currentLane;
}

bool TrafficLight::isGreen(char lane) const {
    return currentLane==lane;
}

void TrafficLight::switchtoNextLane(){
    if(priorityMode){
        currentLane='A';
        return; 
    }
    
    switch (currentLane)
    {
    case 'A':
        currentLane = 'B';
        break;
    case 'B':
        currentLane = 'C';
        break;
    case 'C':
        currentLane = 'D';
        break;
    case 'D':
        currentLane = 'A';
    default:
        break;
    }
}

void TrafficLight::activatePriorityMode(){
    priorityMode = true;
    currentLane = 'A'; 
}

void TrafficLight::deactivatePriorityMode(){
    priorityMode = false;
}

bool TrafficLight::isPriorityMode() const{
    return priorityMode;
}
void TrafficLight::display() const {
    std::cout << "Traffic Light Status:" << std::endl;
    std::cout << "  Lane A: " << (currentLane == 'A' ? "GREEN" : "RED") << std::endl;
    std::cout << "  Lane B: " << (currentLane == 'B' ? "GREEN" : "RED") << std::endl;
    std::cout << "  Lane C: " << (currentLane == 'C' ? "GREEN" : "RED") << std::endl;
    std::cout << "  Lane D: " << (currentLane == 'D' ? "GREEN" : "RED") << std::endl;
    std::cout << "  Mode: " << (priorityMode ? "PRIORITY (Lane A)" : "NORMAL") << std::endl;
}
