#include <iostream>
#include "Vehicle.h"
#include "Queue.h"
#include "TrafficLight.h"
#include "TrafficManager.h"

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "  Traffic Management System Test" << endl;
    cout << "========================================" << endl << endl;
    
    TrafficManager manager;
    
    // Add some vehicles to different lanes
    cout << "=== Adding Vehicles ===" << endl;
    manager.addVehicle(Vehicle("AA1BB111", 'A'));
    manager.addVehicle(Vehicle("AA2BB222", 'A'));
    manager.addVehicle(Vehicle("BB1CC111", 'B'));
    manager.addVehicle(Vehicle("BB2CC222", 'B'));
    manager.addVehicle(Vehicle("BB3CC333", 'B'));
    manager.addVehicle(Vehicle("CC1DD111", 'C'));
    manager.addVehicle(Vehicle("DD1EE111", 'D'));
    manager.addVehicle(Vehicle("DD2EE222", 'D'));
    
    cout << endl;
    
    manager.display();
    
    // Process some cycles
    cout << "\n\n=== Processing Traffic Cycles ===" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "\n>>> Cycle " << (i+1) << " <<<" << endl;
        manager.processCycle();
    }
    
    manager.display();
    
    // Test priority mode
    cout << "\n\n=== Testing Priority Mode ===" << endl;
    cout << "Adding 12 vehicles to Lane A..." << endl;
    for (int i = 0; i < 12; i++) {
        manager.addVehicle(Vehicle("PRIORITY" + to_string(i), 'A'));
    }
    
    manager.display();
    
    // Process until priority resolves
    cout << "\n=== Processing Priority Lane ===" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "\n>>> Cycle " << (i+1) << " <<<" << endl;
        manager.processCycle();
    }
    
    manager.display();
    
    cout << "\n========================================" << endl;
    cout << "  Test Complete!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}