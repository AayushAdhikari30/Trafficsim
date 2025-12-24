#include "TrafficManager.h"
#include "TrafficLight.h"
#include "Vehicle.h"
#include  <iostream>

TrafficManager::TrafficManager() : totalVehiclesProcessed(0) {
}

Queue<Vehicle>& TrafficManager::getLaneQueue(char lane) {
    switch(lane) {
            case'A':
            return laneA;
            case'B':
            return laneB;
            case'C':
            return laneC;
            case'D':
            return laneD;
            default:
            return laneA;
    }
}

void TrafficManager::addVehicle(const Vehicle& vehicle){
    char lane = vehicle.getLaneName();
    Queue<Vehicle>& targetQueue = getLaneQueue(lane);
    targetQueue.enqueue(vehicle);

    std::cout << "Vehicle " << vehicle.getLicensePlate()
              << "added to Lane " << lane 
              << "(Queue size: " << targetQueue.getSize() << ")" << std::endl;
}

int TrafficManager::calculateAverageVehicles() const {
    int total = laneB.getSize() + laneC.getSize() + laneD.getSize();
    int count = 3;

    if (count == 0 ) return 0;
    return total / count;

}
int TrafficManager::getVehiclesToProcess(char lane) const{
    Queue<Vehicle>& laneQueue = const_cast<TrafficManager*>(this)->getLaneQueue(lane);
    if (trafficLight.isPriorityMode() && lane =='A')
    {
        return laneQueue.getSize();
    }
    else{
        int avg = calculateAverageVehicles();
        int queueSize = laneQueue.getSize();
        return (avg < queueSize) ? avg : queueSize;
    }
}

void TrafficManager::checkProiorityMode() {
  int laneASize = laneA.getSize();

if (!trafficLight.isPriorityMode() && laneASize > 10)
    {
        trafficLight.activatePriorityMode();
    }else if (trafficLight.isPriorityMode() && laneASize <5 )
    {
        trafficLight.deactivatePriorityMode();
    }
}

void TrafficManager::processCycle() {

    checkProiorityMode();
    
    char currentLane = trafficLight.getCurrentLane();
    Queue<Vehicle>& currentQueue = getLaneQueue(currentLane);
    
    std::cout << " Lane " << currentLane << " Light is GREEN " << std::endl;
    
    if (currentQueue.isEmpty()) {
        std::cout << "Lane " << currentLane << " is empty. No vehicles to process." << std::endl;
    } else {
        
        int vehiclesToProcess = getVehiclesToProcess(currentLane);
        std::cout << "Processing " << vehiclesToProcess << " vehicles from Lane " << currentLane << std::endl;
        
        for (int i = 0; i < vehiclesToProcess && !currentQueue.isEmpty(); i++) {
            Vehicle v = currentQueue.dequeue();
            std::cout << "  Vehicle " << v.getLicensePlate() << " passed through" << std::endl;
            totalVehiclesProcessed++;
        }
        
        std::cout << "Remaining in Lane " << currentLane << ": " << currentQueue.getSize() << std::endl;
    }
    
   
    trafficLight.switchtoNextLane();
}

    void TrafficManager::display() const {
         std::cout << "Lane A (Priority): " << laneA.getSize() << " vehicles waiting" << std::endl;
    std::cout << "Lane B: " << laneB.getSize() << " vehicles waiting" << std::endl;
    std::cout << "Lane C: " << laneC.getSize() << " vehicles waiting" << std::endl;
    std::cout << "Lane D: " << laneD.getSize() << " vehicles waiting" << std::endl;
    std::cout << "Total vehicles processed: " << totalVehiclesProcessed << std::endl;
    
    std::cout << "\nCurrent Traffic Light:" << std::endl;
    trafficLight.display();
    
    }
   int TrafficManager::getLaneSize(char lane) const {
    switch(lane) {
        case 'A': return laneA.getSize();
        case 'B': return laneB.getSize();
        case 'C': return laneC.getSize();
        case 'D': return laneD.getSize();
        default: return 0;
    }
}
