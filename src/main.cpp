
#include<iostream>
#include"Vehicle.h"

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
    


return 0;

};