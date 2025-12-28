#include "TrafficManager.h"
#include "TrafficLight.h"
#include "Vehicle.h"
#include "SDLRenderer.h"
#include <iostream>

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

void TrafficManager::addVehicle(const Vehicle& vehicle) {
    char road = vehicle.getRoadName();
    int lane = vehicle.getLaneNumber();
    
    
    if (lane != 2) {
        std::cout << "Vehicle " << vehicle.getLicensePlate()
                  << " in Road " << road << " Lane " << lane 
                  << " - NOT MANAGED (only L2 is managed)" << std::endl;
        return;  
    }
    
   
    Queue<Vehicle>& targetQueue = getLaneQueue(road);
    targetQueue.enqueue(vehicle);
    
    std::cout << "Vehicle " << vehicle.getLicensePlate()
              << " added to Road " << road << " Lane 2"
              << " (Queue size: " << targetQueue.getSize() << ")" << std::endl;
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


int TrafficManager::getTotalProcessed() const {
    return totalVehiclesProcessed;
}

void TrafficManager::checkProiorityMode() {
    int laneASize = laneA.getSize();
    
    
    if (!trafficLight.isPriorityMode() && laneASize > 10) {
        trafficLight.activatePriorityMode();
        std::cout << " PRIORITY MODE ACTIVATED - Road A has " 
                  << laneASize << " vehicles!" << std::endl;
    }
    
    else if (trafficLight.isPriorityMode() && laneASize < 5) {
        trafficLight.deactivatePriorityMode();
        std::cout << "✓ Priority mode deactivated - Road A cleared" << std::endl;
    }
}

void TrafficManager::loadVehiclesFromFiles() {
    std::vector<Vehicle> newA, newB, newC, newD;
    FileReader::readAllLaneFiles(newA, newB, newC, newD);
    
    int managedCount = 0;
    int ignoredCount = 0;
    
    
    for (const auto& v : newA) {
        if (v.getLaneNumber() == 2) {
            laneA.enqueue(v);
            managedCount++;
        } else {
            ignoredCount++;
        }
    }
    
    for (const auto& v : newB) {
        if (v.getLaneNumber() == 2) {
            laneB.enqueue(v);
            managedCount++;
        } else {
            ignoredCount++;
        }
    }
    
    for (const auto& v : newC) {
        if (v.getLaneNumber() == 2) {
            laneC.enqueue(v);
            managedCount++;
        } else {
            ignoredCount++;
        }
    }
    
    for (const auto& v : newD) {
        if (v.getLaneNumber() == 2) {
            laneD.enqueue(v);
            managedCount++;
        } else {
            ignoredCount++;
        }
    }
    
    if (managedCount > 0 || ignoredCount > 0) {
        std::cout << " Loaded: " << managedCount << " vehicles (L2), "
                  << "Ignored: " << ignoredCount << " vehicles (L1/L3)" << std::endl;
    }
}

void TrafficManager::processCycle() {
    checkProiorityMode();
    
    char currentRoad = trafficLight.getCurrentLane();
    Queue<Vehicle>& currentQueue = getLaneQueue(currentRoad);
    
    std::cout << "\n Road " << currentRoad << " Light is GREEN" << std::endl;
    
    if (currentQueue.isEmpty()) {
        std::cout << "Road " << currentRoad 
                  << " Lane 2 is empty. No vehicles to process." << std::endl;
    }
    else {
        int vehiclesToProcess = getVehiclesToProcess(currentRoad);
        std::cout << "Processing " << vehiclesToProcess 
                  << " vehicles from Road " << currentRoad << " Lane 2" << std::endl;
        
        for (int i = 0; i < vehiclesToProcess && !currentQueue.isEmpty(); i++) {
            Vehicle v = currentQueue.dequeue();
            std::cout << "  ✓ Vehicle " << v.getLicensePlate() 
                      << " passed through" << std::endl;
            totalVehiclesProcessed++;
        }
        
        std::cout << "Remaining in Road " << currentRoad 
                  << ": " << currentQueue.getSize() << std::endl;
    }
    
    trafficLight.switchToNextLane();
}

void TrafficManager::display() const {
    std::cout << "\n=== Traffic Status ===" << std::endl;
    std::cout << "Road A Lane 2 (Priority): " << laneA.getSize() 
              << " vehicles waiting" << std::endl;
    std::cout << "Road B Lane 2: " << laneB.getSize() 
              << " vehicles waiting" << std::endl;
    std::cout << "Road C Lane 2: " << laneC.getSize() 
              << " vehicles waiting" << std::endl;
    std::cout << "Road D Lane 2: " << laneD.getSize() 
              << " vehicles waiting" << std::endl;
    std::cout << "Total vehicles processed: " << totalVehiclesProcessed << std::endl;
    
    std::cout << "\n";
    trafficLight.display();
}

int TrafficManager::getLaneSize(char road) const {
    switch(road) {
        case 'A': return laneA.getSize();
        case 'B': return laneB.getSize();
        case 'C': return laneC.getSize();
        case 'D': return laneD.getSize();
        default: return 0;
    }
}

void TrafficManager::renderToSDL(SDLRenderer& renderer) const {
    renderer.clear();
    renderer.drawRoad();
    
    char currentRoad = trafficLight.getCurrentLane();
    bool isPriority = trafficLight.isPriorityMode();
    renderer.drawTrafficLight(currentRoad, isPriority);
    
    renderer.drawQueue('A', laneA.getSize());
    renderer.drawQueue('B', laneB.getSize());
    renderer.drawQueue('C', laneC.getSize());
    renderer.drawQueue('D', laneD.getSize());
    
    renderer.drawStats(0, totalVehiclesProcessed, laneA.getSize());
    
    renderer.present();
}