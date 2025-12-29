#include "TrafficManager.h"
#include "TrafficLight.h"
#include "Vehicle.h"
#include "SDLRenderer.h"
#include <iostream>
#include <algorithm>
#include <cmath>

TrafficManager::TrafficManager() : totalVehiclesProcessed(0) {
}

Queue<Vehicle>& TrafficManager::getLaneQueue(char road) {
    switch(road) {
        case 'A': return laneA;
        case 'B': return laneB;
        case 'C': return laneC;
        case 'D': return laneD;
        default: return laneA;
    }
}

std::vector<AnimatedVehicle>& TrafficManager::getActiveVehicles(char road) {
    switch(road) {
        case 'A': return activeVehiclesA;
        case 'B': return activeVehiclesB;
        case 'C': return activeVehiclesC;
        case 'D': return activeVehiclesD;
        default: return activeVehiclesA;
    }
}

const std::vector<AnimatedVehicle>& TrafficManager::getActiveVehicles(char road) const {
    switch(road) {
        case 'A': return activeVehiclesA;
        case 'B': return activeVehiclesB;
        case 'C': return activeVehiclesC;
        case 'D': return activeVehiclesD;
        default: return activeVehiclesA;
    }
}

// Check if a lane is free-flow (lanes 1 and 3 for turning)
bool TrafficManager::isFreeFlowLane(int lane) const {
    return (lane == 1 || lane == 3);  // Lane 1 = right turn, Lane 3 = left turn
}

void TrafficManager::initializeVehiclePosition(Vehicle& vehicle, char road, int queuePosition) {
    const int centerX = 450;
    const int centerY = 350;
    const int roadWidth = 180;
    const int laneWidth = 60;
    const int spacing = 35;
    
    int lane = vehicle.getLaneNumber();
    int laneOffset = (lane - 1) * laneWidth + laneWidth/2;
    
    // Position vehicles OFF-SCREEN initially to prevent backward movement
    if (road == 'A') {
        int startX = -100 - queuePosition * spacing;  // Start far left, off-screen
        int laneY = centerY - roadWidth/2 + laneOffset - 16;
        vehicle.setPosition(startX, laneY);
    }
    else if (road == 'B') {
        int laneX = centerX - roadWidth/2 + laneOffset - 16;
        int startY = -100 - queuePosition * spacing;  // Start far up, off-screen
        vehicle.setPosition(laneX, startY);
    }
    else if (road == 'C') {
        int startX = 1000 + queuePosition * spacing;  // Start far right, off-screen
        int laneY = centerY - roadWidth/2 + laneOffset - 16;
        vehicle.setPosition(startX, laneY);
    }
    else if (road == 'D') {
        int laneX = centerX - roadWidth/2 + laneOffset - 16;
        int startY = 800 + queuePosition * spacing;  // Start far down, off-screen
        vehicle.setPosition(laneX, startY);
    }
}

void TrafficManager::setVehicleWaitingPosition(Vehicle& vehicle, char road, int queuePosition) {
    const int centerX = 450;
    const int centerY = 350;
    const int roadWidth = 180;
    const int laneWidth = 60;
    const int spacing = 35;
    const int stopDistance = roadWidth/2 + 20;
    
    int lane = vehicle.getLaneNumber();
    int laneOffset = (lane - 1) * laneWidth + laneWidth/2;
    
    // Free-flow lanes (1 and 3) go straight through without stopping
    if (isFreeFlowLane(lane)) {
        setVehicleMovingThroughIntersection(vehicle, road);
        return;
    }
    
    // Lane 2 (middle lane) must stop and wait for green light
    if (road == 'A') {
        int laneY = centerY - roadWidth/2 + laneOffset - 16;
        float targetX = centerX - stopDistance - queuePosition * spacing;
        vehicle.setTarget(targetX, laneY);
    }
    else if (road == 'B') {
        int laneX = centerX - roadWidth/2 + laneOffset - 16;
        float targetY = centerY - stopDistance - queuePosition * spacing;
        vehicle.setTarget(laneX, targetY);
    }
    else if (road == 'C') {
        int laneY = centerY - roadWidth/2 + laneOffset - 16;
        float targetX = centerX + stopDistance + queuePosition * spacing;
        vehicle.setTarget(targetX, laneY);
    }
    else if (road == 'D') {
        int laneX = centerX - roadWidth/2 + laneOffset - 16;
        float targetY = centerY + stopDistance + queuePosition * spacing;
        vehicle.setTarget(laneX, targetY);
    }
    
    vehicle.setMoving(true);
    vehicle.setSpeed(80.0f);
}



void TrafficManager::addVehicle(const Vehicle& vehicle) {
    char road = vehicle.getRoadName();
    int lane = vehicle.getLaneNumber();
    
    Queue<Vehicle>& targetQueue = getLaneQueue(road);
    targetQueue.enqueue(vehicle);
    
    std::cout << "Vehicle " << vehicle.getLicensePlate()
              << " added to Road " << road << " Lane " << lane;
    
    if (isFreeFlowLane(lane)) {
        std::cout << " (FREE FLOW)";
    }
    std::cout << " (Queue: " << targetQueue.getSize() << ")" << std::endl;
}

void TrafficManager::spawnQueuedVehicles() {
    char roads[] = {'A', 'B', 'C', 'D'};
    
    for (char road : roads) {
        Queue<Vehicle>& queue = getLaneQueue(road);
        std::vector<AnimatedVehicle>& activeVehicles = getActiveVehicles(road);
        
        const int maxActive = 15;
        int activeCount = activeVehicles.size();
        
        while (!queue.isEmpty() && activeCount < maxActive) {
            Vehicle v = queue.dequeue();
            
            // Initialize vehicle far off-screen
            initializeVehiclePosition(v, road, activeCount);
            
            // Set target based on lane type
            setVehicleWaitingPosition(v, road, activeCount);
            
            AnimatedVehicle av(v);
            
            // Free-flow lanes are already moving through
            if (isFreeFlowLane(v.getLaneNumber())) {
                av.hasPassedIntersection = true;
            }
            
            activeVehicles.push_back(av);
            activeCount++;
        }
    }
}

void TrafficManager::updateVehiclePositions(float deltaTime)
{
    char roads[] = {'A', 'B', 'C', 'D'};

    for (char road : roads) {
        std::vector<AnimatedVehicle>& vehicles = getActiveVehicles(road);

        for (auto& av : vehicles) {
            av.vehicle.updatePosition(deltaTime);

            if (av.vehicle.hasReachedTarget()) {

                // continue turn
                if (av.vehicle.getTurnStage() == 1) {
                    setVehicleMovingThroughIntersection(av.vehicle, road);
                }
                else {
                    av.isActive = false;
                    av.hasPassedIntersection = true;
                }
            }
        }
    }
}


void TrafficManager::cleanupInactiveVehicles() {
    char roads[] = {'A', 'B', 'C', 'D'};
    
    for (char road : roads) {
        std::vector<AnimatedVehicle>& vehicles = getActiveVehicles(road);
        
        vehicles.erase(
            std::remove_if(vehicles.begin(), vehicles.end(),
                [](const AnimatedVehicle& av) { return !av.isActive; }),
            vehicles.end()
        );
    }
}

int TrafficManager::calculateAverageVehicles() const {
    int total = laneB.getSize() + laneC.getSize() + laneD.getSize();
    int count = 3;
    
    if (count == 0) return 0;
    return total / count;
}

char TrafficManager::getCurrentLane() const {
    return trafficLight.getCurrentLane();
}

int TrafficManager::getVehiclesToProcess(char road) const {
    Queue<Vehicle>& laneQueue = const_cast<TrafficManager*>(this)->getLaneQueue(road);
    
    if (trafficLight.isPriorityMode() && road == 'A') {
        return laneQueue.getSize();
    }
    else {
        int avg = calculateAverageVehicles();
        int queueSize = laneQueue.getSize();
        return (avg < queueSize) ? avg : queueSize;
    }
}

void TrafficManager::checkProiorityMode() {
    int laneASize = laneA.getSize() + activeVehiclesA.size();
    
    if (!trafficLight.isPriorityMode() && laneASize > 10) {
        trafficLight.activatePriorityMode();
        std::cout << "\n PRIORITY MODE ACTIVATED - Road A has " 
                  << laneASize << " vehicles!\n" << std::endl;
    }
    else if (trafficLight.isPriorityMode() && laneASize < 5) {
        trafficLight.deactivatePriorityMode();
        std::cout << "\nPriority mode deactivated - Road A cleared\n" << std::endl;
    }
}

void TrafficManager::processCycle() {
    checkProiorityMode();
    
    char currentRoad = trafficLight.getCurrentLane();
    std::vector<AnimatedVehicle>& currentVehicles = getActiveVehicles(currentRoad);
    
    std::cout << "\n Traffic Light Road " << currentRoad << " is GREEN" << std::endl;
    
    int waitingCount = 0;
    for (const auto& av : currentVehicles) {
        if (!av.hasPassedIntersection && !isFreeFlowLane(av.vehicle.getLaneNumber())) {
            waitingCount++;
        }
    }
    
    if (waitingCount == 0) {
        std::cout << "   No vehicles waiting at light on Road " << currentRoad << std::endl;
    }
    else {
        int processed = 0;
        
        // Only process vehicles in lane 2 (middle lane) that must obey lights
        for (auto& av : currentVehicles) {
            if (!av.hasPassedIntersection && av.vehicle.getLaneNumber() == 2) {
                if (!av.vehicle.getIsMoving() || av.vehicle.hasReachedTarget()) {
                    setVehicleMovingThroughIntersection(av.vehicle, currentRoad);
                    av.hasPassedIntersection = true;
                    processed++;
                    totalVehiclesProcessed++;
                }
            }
        }
        
        std::cout << "   Released " << processed << " vehicle(s) from Road " 
                  << currentRoad << " (Lane 2 only)" << std::endl;
        std::cout << "   Remaining at light: " << (waitingCount - processed) 
                  << " vehicle(s)" << std::endl;
    }
    
    trafficLight.switchToNextLane();
}

void TrafficManager::loadVehiclesFromFiles() {
    std::vector<Vehicle> newA, newB, newC, newD;
    FileReader::readAllLaneFiles(newA, newB, newC, newD);
    
    int loadedCount = 0;
    
    for (const auto& v : newA) {
        laneA.enqueue(v);
        loadedCount++;
    }
    
    for (const auto& v : newB) {
        laneB.enqueue(v);
        loadedCount++;
    }
    
    for (const auto& v : newC) {
        laneC.enqueue(v);
        loadedCount++;
    }
    
    for (const auto& v : newD) {
        laneD.enqueue(v);
        loadedCount++;
    }
    
    if (loadedCount > 0) {
        std::cout << "Loaded " << loadedCount << " new vehicle(s) from files" << std::endl;
    }
}

void TrafficManager::display() const {
    std::cout << "Road A (Priority): " << laneA.getSize() 
              << " queued + " << activeVehiclesA.size() << " active" << std::endl;
    std::cout << "Road B:           " << laneB.getSize() 
              << " queued + " << activeVehiclesB.size() << " active" << std::endl;
    std::cout << "Road C:           " << laneC.getSize() 
              << " queued + " << activeVehiclesC.size() << " active" << std::endl;
    std::cout << "Road D:           " << laneD.getSize() 
              << " queued + " << activeVehiclesD.size() << " active" << std::endl;
    std::cout << "\nTotal Processed: " << totalVehiclesProcessed << " vehicles" << std::endl;
    
    std::cout << "\n";
    trafficLight.display();
}

int TrafficManager::getLaneSize(char road) const {
    switch(road) {
        case 'A': return laneA.getSize() + activeVehiclesA.size();
        case 'B': return laneB.getSize() + activeVehiclesB.size();
        case 'C': return laneC.getSize() + activeVehiclesC.size();
        case 'D': return laneD.getSize() + activeVehiclesD.size();
        default: return 0;
    }
}

int TrafficManager::getTotalProcessed() const {
    return totalVehiclesProcessed;
}

void TrafficManager::renderToSDL(SDLRenderer& renderer) const {
    renderer.clear();
    renderer.drawRoad();
    
    char currentRoad = trafficLight.getCurrentLane();
    bool isPriority = trafficLight.isPriorityMode();
    renderer.drawTrafficLight(currentRoad, isPriority);
    
    char roads[] = {'A', 'B', 'C', 'D'};
    for (char road : roads) {
        const std::vector<AnimatedVehicle>& vehicles = getActiveVehicles(road);
        for (const auto& av : vehicles) {
            renderer.drawAnimatedVehicle(
                av.vehicle.getX(), 
                av.vehicle.getY(), 
                road,
                av.vehicle.getLaneNumber()
            );
        }
    }
    
    renderer.drawStats(0, totalVehiclesProcessed, laneA.getSize());
    renderer.present();
}
void TrafficManager::setVehicleMovingThroughIntersection(Vehicle& vehicle, char road)
{
    const int centerX = 450;
    const int centerY = 350;

    int lane = vehicle.getLaneNumber();

    // STEP 1: always go to intersection center first
    if (vehicle.getTurnStage() == 0) {
        vehicle.setTarget(centerX, centerY);
        vehicle.setMoving(true);
        vehicle.setSpeed(120.0f);
        vehicle.setTurnStage(1);
        return;
    }

    // STEP 2: exit based on road + lane
    // Lane 1 = LEFT, Lane 2 = STRAIGHT, Lane 3 = RIGHT

    if (road == 'A') { // left → right
        if (lane == 1) vehicle.setTarget(centerX, -50);      // LEFT → up
        if (lane == 2) vehicle.setTarget(950, centerY);      // straight
        if (lane == 3) vehicle.setTarget(centerX, 750);      // RIGHT → down
    }
    else if (road == 'B') { // top → bottom
        if (lane == 1) vehicle.setTarget(950, centerY);      // LEFT → right
        if (lane == 2) vehicle.setTarget(centerX, 750);      // straight
        if (lane == 3) vehicle.setTarget(-50, centerY);      // RIGHT → left
    }
    else if (road == 'C') { // right → left
        if (lane == 1) vehicle.setTarget(centerX, 750);      // LEFT → down
        if (lane == 2) vehicle.setTarget(-50, centerY);      // straight
        if (lane == 3) vehicle.setTarget(centerX, -50);      // RIGHT → up
    }
    else if (road == 'D') { // bottom → top
        if (lane == 1) vehicle.setTarget(-50, centerY);      // LEFT → left
        if (lane == 2) vehicle.setTarget(centerX, -50);      // straight
        if (lane == 3) vehicle.setTarget(950, centerY);      // RIGHT → right
    }

    vehicle.setMoving(true);
    vehicle.setSpeed(120.0f);
}
