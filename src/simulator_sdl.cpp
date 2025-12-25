#include <iostream>
#include <windows.h>
#include "SDLRenderer.h"
#include "TrafficManager.h"
#include "Vehicle.h"
#include <thread>
#include <chrono>

int main() {
    std::cout << "Initializing SDL Traffic Simulator..." << std::endl;
    
    SDLRenderer renderer(800, 600);
    if (!renderer.init("Traffic Management System - SDL3")) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return 1;
    }
    
    std::cout << "SDL initialized successfully!" << std::endl;
    std::cout << "Run the generator in another terminal!" << std::endl;
    
    TrafficManager manager;
    int cycleCount = 0;
    
    auto lastCycleTime = std::chrono::steady_clock::now();
    const int CYCLE_INTERVAL_MS = 3000;
    
    // Frame timing
    auto lastFrameTime = std::chrono::steady_clock::now();
    const int TARGET_FPS = 60;
    const int FRAME_TIME_MS = 1000 / TARGET_FPS;
    
    while (!renderer.shouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();
        
        // IMPORTANT: Handle events every frame to prevent "not responding"
        renderer.handleEvents();
        
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastCycleTime
        ).count();
        
        // Process cycle every 3 seconds
        if (elapsed >= CYCLE_INTERVAL_MS) {
            cycleCount++;
            
            std::cout << "\n=== CYCLE " << cycleCount << " ===" << std::endl;
            
            // Load vehicles
            manager.loadVehiclesFromFiles();
            
            // Process cycle
            manager.processCycle();
            
            lastCycleTime = now;
        }
        
        // Render
        manager.renderToSDL(renderer);
        
       
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
    
    std::cout << "Simulator closed." << std::endl;
    return 0;
}