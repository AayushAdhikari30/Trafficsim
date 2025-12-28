#include <iostream>
#include <windows.h>
#include "SDLRenderer.h"
#include "TrafficManager.h"
#include "Vehicle.h"
#include <thread>
#include <chrono>

int main() {
   
    std::cout << "  Traffic Management System \n";

    std::cout << "Initializing SDL..." << std::endl;
    
    SDLRenderer renderer(900, 700);  
    if (!renderer.init("Traffic Management System - 3 Lane Simulator")) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return 1;
    }
    
    std::cout << "✓ SDL initialized successfully!\n";
    std::cout << "✓ Window created (900x700)\n";
    std::cout << "\n Instructions:\n";
    std::cout << "   - Run generator_main.exe in another terminal\n";
    std::cout << "   - Watch vehicles appear in Lane 2 (middle lane)\n";
    std::cout << "   - Priority mode activates when Road A > 10 vehicles\n";
    std::cout << "   - Close window to exit\n\n";
    
    TrafficManager manager;
    int cycleCount = 0;
    
    auto lastCycleTime = std::chrono::steady_clock::now();
    const int CYCLE_INTERVAL_MS = 3000; 
    
    auto lastLoadTime = std::chrono::steady_clock::now();
    const int LOAD_INTERVAL_MS = 500;   
    
    
    const int TARGET_FPS = 60;
    const int FRAME_TIME_MS = 1000 / TARGET_FPS;
    
    std::cout << "Starting simulation loop...\n";
    std::cout << "Cycle interval: " << CYCLE_INTERVAL_MS / 1000 << " seconds\n\n";
    
    while (!renderer.shouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();
       
        renderer.handleEvents();
        
        auto now = std::chrono::steady_clock::now();
        
       
        auto loadElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastLoadTime
        ).count();
        
        if (loadElapsed >= LOAD_INTERVAL_MS) {
            manager.loadVehiclesFromFiles();
            lastLoadTime = now;
        }
        
       
        auto cycleElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastCycleTime
        ).count();
        
        if (cycleElapsed >= CYCLE_INTERVAL_MS) {
            cycleCount++;
            
           
            
            manager.processCycle();
            
            
            std::cout << "\n";
            manager.display();
            
            lastCycleTime = now;
        }
        
       
        renderer.clear();
        renderer.drawRoad();
        
        char currentLane = manager.getCurrentLane();
        bool isPriority = (manager.getLaneSize('A') > 10);
        
        renderer.drawTrafficLight(currentLane, isPriority);
        
       
        renderer.drawQueue('A', manager.getLaneSize('A'));
        renderer.drawQueue('B', manager.getLaneSize('B'));
        renderer.drawQueue('C', manager.getLaneSize('C'));
        renderer.drawQueue('D', manager.getLaneSize('D'));
        
     renderer.drawStats(cycleCount, manager.getTotalProcessed(), manager.getLaneSize('A'));
        
        renderer.present();
         
       
        auto frameEnd = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
            frameEnd - frameStart
        ).count();
        
        if (frameDuration < FRAME_TIME_MS) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(FRAME_TIME_MS - frameDuration)
            );
        }
    }
    
  
    
    return 0;
}