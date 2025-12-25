#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include "Vehicle.h"
#include "Queue.h"
#include "TrafficLight.h"
#include "TrafficManager.h"

using namespace std;
using namespace std::chrono_literals;

int main() {
    cout << "========================================" << endl;
    cout << "  Traffic Management Simulator" << endl;
    cout << "========================================" << endl << endl;
    
    TrafficManager manager;
    
    cout << "Starting simulation..." << endl;
    cout << "The simulator will read vehicles from lane files" << endl;
    cout << "Run the generator in another terminal!" << endl;
    cout << "Press Ctrl+C to stop\n" << endl;
    
    int cycleCount = 0;
    
    while (true) {
        cycleCount++;
        
        cout << "\n========================================" << endl;
        cout << "CYCLE " << cycleCount << endl;
        cout << "========================================" << endl;
        
        // Load new vehicles from files
        cout << "\n--- Loading vehicles from files ---" << endl;
        manager.loadVehiclesFromFiles();
        
        // Show current status
        cout << "\n--- Current Status ---" << endl;
        manager.display();
        
        // Process one traffic cycle
        cout << "\n--- Processing Traffic ---" << endl;
        manager.processCycle();
        
        // Wait 3 seconds before next cycle
        cout << "\nWaiting 3 seconds..." << endl;
       Sleep(3000);
    }
    
    return 0;
}