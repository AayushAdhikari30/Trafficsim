#include "TrafficGenerator.h"
#include <iostream>

int main() {
    TrafficGenerator generator;
    
    std::cout << "\nStarting continuous generation...\n" << std::endl;
    
  
    generator.run();
    
    return 0;
}