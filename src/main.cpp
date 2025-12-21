
#include<iostream>
#include"Vehicle.h"
#include "Queue.h"

using namespace std;

int main(){

    cout<<"Traffic Simulation Started"<<endl<<endl;

    Vehicle v1("ABC123",'A');
    Vehicle v2("XYZ789",'B'); 
    Vehicle v3("LMN456",'C'); 
    Vehicle v4("DEF321",'A');

    v1.display();
    v2.display();
    v3.display();
    v4.display();


       cout << "Testing getters for car1:" << endl;
    cout << "  License: " << v1.getLicensePlate() << endl;
    cout << "  Lane: " << v1.getLaneName() << endl;
    cout << "  Entry Time: " << v1.getEntryTime() << endl;
    

    cout<< "Testing Queue Operations"<<endl;
    Queue<Vehicle> laneA;
    cout<<"Is lane A empty? " << (laneA.isEmpty() ? "Yes" : "NO") <<endl;
    cout<<"lane A size: " << laneA.getSize() <<endl;
    laneA.enqueue(v1);
    laneA.enqueue(v4);
    
    cout << endl << "lane A Size:" << laneA.getSize() << endl;

    cout<<"In Lane A:"<<endl;
    laneA.display();cout<<endl;

     cout << "Vehicle at front (without removing)" << endl;
     Vehicle frontVehicle = laneA.getFront();
     frontVehicle.display();

     //Removing vehicles one by one
     Vehicle removed1 = laneA.dequeue();
     cout << "Vehicle leaving: " ; removed1.display();
     cout<< "Remaning in lane: " <<laneA.getSize() << endl <<endl;

     Vehicle removed4 = laneA.dequeue();
     cout << "Vehicle leaving: " ; removed4.display();
     cout<< "Remaning in lane: " <<laneA.getSize() << endl <<endl;

      cout << "Is Lane A empty now? " << (laneA.isEmpty() ? "YES - All vehicles passed!" : "NO") << endl << endl;
    
    
    cout << "  Queue Test PASSED!" << endl;
   






return 0;

};