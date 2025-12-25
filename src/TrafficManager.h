#ifndef TrafficManager_H
#define TrafficManager_H

#include "Queue.h"
#include "TrafficLight.h"
#include "Vehicle.h"
#include <vector>
#include "FileReader.h" 

using namespace std;

class TrafficManager {
    private:
        Queue<Vehicle> laneA;
        Queue<Vehicle> laneB;
        Queue<Vehicle> laneC;
        Queue<Vehicle> laneD;

        
      int totalVehiclesProcessed;
     
      int getVehiclesToProcess(char lane) const;

        TrafficLight trafficLight;
    Queue<Vehicle>& getLaneQueue(char lane);

    int calculateAverageVehicles() const;
    int getVehicleCount(char lane) const;
    


    public:
    char getCurrentLane() const;
     int getLaneSize(char lane) const;
        TrafficManager();


        void loadVehiclesFromFiles();
        void addVehicle(const Vehicle& vehicle);

        void processCycle();

        void checkProiorityMode();

        void display() const;

        void getLaneStatus(char lane) const;
        
        
};



#endif