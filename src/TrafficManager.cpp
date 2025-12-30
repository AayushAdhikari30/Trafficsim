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
    const int spacing = 40;
    
    int lane = vehicle.getLaneNumber();
    int laneOffset = (lane - 1) * laneWidth + laneWidth/2;
    
    // Position vehicles OFF-SCREEN in correct lanes
    if (road == 'A') {
        // Horizontal road: lane offset is vertical position
        int startX = -100 - queuePosition * spacing;
        int laneY = centerY - roadWidth/2 + laneOffset;
        vehicle.setPosition(startX, laneY);
    }
    else if (road == 'B') {
        // Vertical road: lane offset is horizontal position
        int laneX = centerX - roadWidth/2 + laneOffset;
        int startY = -100 - queuePosition * spacing;
        vehicle.setPosition(laneX, startY);
    }
    else if (road == 'C') {
        // Horizontal road: lane offset is vertical position
        int startX = 1000 + queuePosition * spacing;
        int laneY = centerY - roadWidth/2 + laneOffset;
        vehicle.setPosition(startX, laneY);
    }
    else if (road == 'D') {
        // Vertical road: lane offset is horizontal position
        int laneX = centerX - roadWidth/2 + laneOffset;
        int startY = 800 + queuePosition * spacing;
        vehicle.setPosition(laneX, startY);
    }
}

void TrafficManager::setVehicleWaitingPosition(Vehicle& vehicle, char road, int queuePosition) {
    const int centerX = 450;
    const int centerY = 350;
    const int roadWidth = 180;
    const int laneWidth = 60;
    const int carLength = 35;  // Car length for proper spacing
    const int stopDistance = roadWidth/2 + 20;
    
    int lane = vehicle.getLaneNumber();
    int laneOffset = (lane - 1) * laneWidth + laneWidth/2;
    
    // ALL lanes converge to the stop line
    // Cars queue up one behind another (only for lane 2)
    
    if (road == 'A') {
        // Horizontal road: maintain lane Y position, cars queue backward from stop line
        int laneY = centerY - roadWidth/2 + laneOffset;
        float targetX = centerX - stopDistance - queuePosition * carLength;
        vehicle.setTarget(targetX, laneY);
    }
    else if (road == 'B') {
        // Vertical road: maintain lane X position, cars queue upward from stop line
        int laneX = centerX - roadWidth/2 + laneOffset;
        float targetY = centerY - stopDistance - queuePosition * carLength;
        vehicle.setTarget(laneX, targetY);
    }
    else if (road == 'C') {
        // Horizontal road: maintain lane Y position, cars queue forward from stop line
        int laneY = centerY - roadWidth/2 + laneOffset;
        float targetX = centerX + stopDistance + queuePosition * carLength;
        vehicle.setTarget(targetX, laneY);
    }
    else if (road == 'D') {
        // Vertical road: maintain lane X position, cars queue downward from stop line
        int laneX = centerX - roadWidth/2 + laneOffset;
        float targetY = centerY + stopDistance + queuePosition * carLength;
        vehicle.setTarget(laneX, targetY);
    }
    
    vehicle.setMoving(true);
    vehicle.setSpeed(80.0f);
    
    // Only lane 2 stops; lanes 1 and 3 continue
    if (!isFreeFlowLane(lane)) {
        vehicle.setAtStop(true);  // Lane 2: waiting for light
    }
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
            
            // For lane 2, count only lane 2 vehicles waiting at stop line for proper queue position
            int queuePosition = 0;
            if (v.getLaneNumber() == 2) {
                for (const auto& av : activeVehicles) {
                    if (av.vehicle.getLaneNumber() == 2 && av.vehicle.getAtStop()) {
                        queuePosition++;
                    }
                }
            }
            
            // Initialize vehicle far off-screen
            initializeVehiclePosition(v, road, queuePosition);
            
            // Set target to move toward stop line
            setVehicleWaitingPosition(v, road, queuePosition);
            
            AnimatedVehicle av(v);
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
            // If waiting at stop line (lane 2), don't move
            if (av.vehicle.getAtStop() && !trafficLight.isGreen(road)) {
                continue;
            }
            
            av.vehicle.updatePosition(deltaTime);

            if (av.vehicle.hasReachedTarget()) {
                // If reached stop line and this is a free-flow lane (1 or 3), proceed through
                if (isFreeFlowLane(av.vehicle.getLaneNumber()) && av.vehicle.getTurnStage() == 0) {
                    setVehicleMovingThroughIntersection(av.vehicle, road);
                }
                // If reached stop line and this is lane 2, wait for green light (marked as atStop)
                // When light turns green, processCycle will release it
                else if (av.vehicle.getTurnStage() == 1) {
                    // Completing the turn through intersection
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
    
    // Count waiting vehicles in lane 2 (straight only)
    int waitingCount = 0;
    AnimatedVehicle* firstWaitingCar = nullptr;
    
    for (auto& av : currentVehicles) {
        if (!av.hasPassedIntersection && av.vehicle.getLaneNumber() == 2 && av.vehicle.getAtStop()) {
            waitingCount++;
            if (firstWaitingCar == nullptr) {
                firstWaitingCar = &av;
            }
        }
    }
    
    if (waitingCount == 0) {
        std::cout << "   No vehicles waiting at light on Road " << currentRoad << std::endl;
    }
    else {
        // Release only ONE car per cycle (realistic traffic management)
        if (firstWaitingCar != nullptr) {
            setVehicleMovingThroughIntersection(firstWaitingCar->vehicle, currentRoad);
            firstWaitingCar->vehicle.setAtStop(false);
            firstWaitingCar->hasPassedIntersection = true;
            totalVehiclesProcessed++;
            
            std::cout << "   Released 1 vehicle from Road " << currentRoad 
                      << " (Remaining: " << (waitingCount - 1) << " waiting)" << std::endl;
        }
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
    const int laneWidth = 60;

    int lane = vehicle.getLaneNumber();
    int laneOffset = (lane - 1) * laneWidth + laneWidth/2;

    // STEP 1: Move to intersection while staying in lane, then exit
    if (vehicle.getTurnStage() == 0) {
        // Calculate intersection entry point based on lane
        float intersectionX, intersectionY;
        
        if (road == 'A' || road == 'C') {
            // Horizontal roads: enter at lane Y
            intersectionX = centerX;
            intersectionY = centerY - laneWidth + laneOffset;
        } else {
            // Vertical roads: enter at lane X
            intersectionX = centerX - laneWidth + laneOffset;
            intersectionY = centerY;
        }
        
        vehicle.setTarget(intersectionX, intersectionY);
        vehicle.setMoving(true);
        vehicle.setSpeed(120.0f);
        vehicle.setTurnStage(1);
        return;
    }

    // STEP 2: Exit based on road + lane (maintain lane separation)
    if (road == 'A') { // vehicles entering from LEFT, going RIGHT
        // Lane 1 (top): LEFT turn → exit UP in top lane
        if (lane == 1) vehicle.setTarget(centerX - 90, -50);
        // Lane 2 (middle): STRAIGHT → exit RIGHT in middle lane
        if (lane == 2) vehicle.setTarget(950, centerY);
        // Lane 3 (bottom): RIGHT turn → exit DOWN in bottom lane
        if (lane == 3) vehicle.setTarget(centerX + 90, 750);
    }
    else if (road == 'B') { // vehicles entering from TOP, going DOWN
        // Lane 1 (left): LEFT turn → exit RIGHT in right lane
        if (lane == 1) vehicle.setTarget(950, centerY - 90);
        // Lane 2 (middle): STRAIGHT → exit DOWN in middle lane
        if (lane == 2) vehicle.setTarget(centerX, 750);
        // Lane 3 (right): RIGHT turn → exit LEFT in left lane
        if (lane == 3) vehicle.setTarget(-50, centerY + 90);
    }
    else if (road == 'C') { // vehicles entering from RIGHT, going LEFT
        // Lane 1 (bottom): LEFT turn → exit DOWN in bottom lane
        if (lane == 1) vehicle.setTarget(centerX + 90, 750);
        // Lane 2 (middle): STRAIGHT → exit LEFT in middle lane
        if (lane == 2) vehicle.setTarget(-50, centerY);
        // Lane 3 (top): RIGHT turn → exit UP in top lane
        if (lane == 3) vehicle.setTarget(centerX - 90, -50);
    }
    else if (road == 'D') { // vehicles entering from BOTTOM, going UP
        // Lane 1 (right): LEFT turn → exit LEFT in left lane
        if (lane == 1) vehicle.setTarget(-50, centerY + 90);
        // Lane 2 (middle): STRAIGHT → exit UP in middle lane
        if (lane == 2) vehicle.setTarget(centerX, -50);
        // Lane 3 (left): RIGHT turn → exit RIGHT in right lane
        if (lane == 3) vehicle.setTarget(950, centerY - 90);
    }

    vehicle.setMoving(true);
    vehicle.setSpeed(120.0f);
}
