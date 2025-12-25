#include "TrafficManager.h"
#include "TrafficLight.h"
#include "Vehicle.h"
#include "SDLRenderer.h"
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
char TrafficManager::getCurrentLane() const {
    return trafficLight.getCurrentLane();
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

void TrafficManager::loadVehiclesFromFiles() {
    std::vector<Vehicle> newA, newB, newC, newD;
    FileReader::readAllLaneFiles(newA, newB, newC, newD);
    
    // Add vehicles to queues
    for (const auto& v : newA) {
        laneA.enqueue(v);
        std::cout << "Loaded: " << v.getLicensePlate() << " Lane A" << std::endl;
    }
    for (const auto& v : newB) {
        laneB.enqueue(v);
        std::cout << "Loaded: " << v.getLicensePlate() << " Lane B" << std::endl;
    }
    for (const auto& v : newC) {
        laneC.enqueue(v);
        std::cout << "Loaded: " << v.getLicensePlate() << " Lane C" << std::endl;
    }
    for (const auto& v : newD) {
        laneD.enqueue(v);
        std::cout << "Loaded: " << v.getLicensePlate() << " Lane D" << std::endl;
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
    
   
    trafficLight.switchToNextLane();
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
void TrafficManager::renderToSDL(SDLRenderer& renderer) const {
    renderer.clear();
    
   
    renderer.drawRoad();
    
   
    char currentLane = trafficLight.getCurrentLane();
    bool isPriority = trafficLight.isPriorityMode();
    renderer.drawTrafficLight(currentLane, isPriority);
    
 
    renderer.drawQueue('A', laneA.getSize());
    renderer.drawQueue('B', laneB.getSize());
    renderer.drawQueue('C', laneC.getSize());
    renderer.drawQueue('D', laneD.getSize());
    
   
    renderer.drawStats(0, totalVehiclesProcessed, 0);
    
    renderer.present();
}
