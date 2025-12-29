#ifndef TRAFFICMANAGER_H
#define TRAFFICMANAGER_H

#include "Queue.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "FileReader.h"
#include <vector>

class SDLRenderer;

struct AnimatedVehicle {
    Vehicle vehicle;
    bool isActive;
    bool hasPassedIntersection;
    
    AnimatedVehicle(const Vehicle& v) 
        : vehicle(v), isActive(true), hasPassedIntersection(false) {}
};

class TrafficManager {
private:
    Queue<Vehicle> laneA;
    Queue<Vehicle> laneB;
    Queue<Vehicle> laneC;
    Queue<Vehicle> laneD;
    
    std::vector<AnimatedVehicle> activeVehiclesA;
    std::vector<AnimatedVehicle> activeVehiclesB;
    std::vector<AnimatedVehicle> activeVehiclesC;
    std::vector<AnimatedVehicle> activeVehiclesD;
    
    TrafficLight trafficLight;
    int totalVehiclesProcessed;
    
    Queue<Vehicle>& getLaneQueue(char road);
    std::vector<AnimatedVehicle>& getActiveVehicles(char road);
    const std::vector<AnimatedVehicle>& getActiveVehicles(char road) const;
    
    void initializeVehiclePosition(Vehicle& vehicle, char road, int queuePosition);
    void setVehicleWaitingPosition(Vehicle& vehicle, char road, int queuePosition);
    void setVehicleMovingThroughIntersection(Vehicle& vehicle, char road);
    
    int calculateAverageVehicles() const;
    void checkProiorityMode();
    
   
    bool isFreeFlowLane(int lane) const;
    
public:
    TrafficManager();
    
    void addVehicle(const Vehicle& vehicle);
    void spawnQueuedVehicles();
    void updateVehiclePositions(float deltaTime);
    void cleanupInactiveVehicles();
    
    void processCycle();
    void display() const;
    void loadVehiclesFromFiles();
    
    char getCurrentLane() const;
    int getVehiclesToProcess(char road) const;
    int getLaneSize(char road) const;
    int getTotalProcessed() const;
    
    void renderToSDL(SDLRenderer& renderer) const;
};

#endif