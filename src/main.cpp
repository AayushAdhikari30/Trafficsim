#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include "Vehicle.h"
#include "Queue.h"
#include "TrafficLight.h"
#include "TrafficManager.h"

using namespace std;


int main() {
   
    cout << "  Traffic Management Simulator" << endl;
  
    
    TrafficManager manager;
    
    cout << "Starting simulation..." << endl;
   
   
    cout << "Press Ctrl+C to stop\n" << endl;
    
    int cycleCount = 0;
    
    while (true) {
        cycleCount++;
        
       
      
        
       
       
        manager.loadVehiclesFromFiles();
        
       
        cout << "\n--- Current Status ---" << endl;
        manager.display();
        
       
        cout << "\n--- Processing Traffic ---" << endl;
        manager.processCycle();
        
       
        cout << "\nWaiting 3 seconds..." << endl;
       Sleep(3000);
    }
    
    return 0;
}