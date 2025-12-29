#define SDL_MAIN_HANDLED
#include <iostream>
#include <windows.h>
#include "SDLRenderer.h"
#include "TrafficManager.h"
#include "Vehicle.h"
#include <thread>
#include <chrono>

int main() {
    SDLRenderer renderer(900, 700);
    if (!renderer.init("Traffic Management System - Animated")) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return 1;
    }
    
    std::cout << "✓ SDL initialized successfully!\n";
    std::cout << "✓ Window created (900x700)\n";
    std::cout << "\n🚦 Traffic Rules:\n";
    std::cout << "   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "   Lane 1 (Right):  🟢 FREE FLOW - Right turns\n";
    std::cout << "   Lane 2 (Middle): 🔴 CONTROLLED - Obeys lights\n";
    std::cout << "   Lane 3 (Left):   🟢 FREE FLOW - Left turns\n";
    std::cout << "   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    std::cout << "📋 Instructions:\n";
    std::cout << "   - Run generator_main.exe in another terminal\n";
    std::cout << "   - Vehicles in lanes 1 & 3 flow freely (no stops)\n";
    std::cout << "   - Vehicles in lane 2 stop at red lights\n";
    std::cout << "   - Priority mode: Road A > 10 vehicles\n";
    std::cout << "   - Close window to exit\n\n";
    
    TrafficManager manager;
    int cycleCount = 0;
    
    auto lastCycleTime = std::chrono::steady_clock::now();
    const int CYCLE_INTERVAL_MS = 5000; 
    
    auto lastLoadTime = std::chrono::steady_clock::now();
    const int LOAD_INTERVAL_MS = 500;   
    
    auto lastSpawnTime = std::chrono::steady_clock::now();
    const int SPAWN_INTERVAL_MS = 100;  
    
    const int TARGET_FPS = 60;
    const int FRAME_TIME_MS = 1000 / TARGET_FPS;
    
    std::cout << "⏱️  Light cycle: " << CYCLE_INTERVAL_MS / 1000 << " seconds\n";
    std::cout << "🎮 Simulation running at " << TARGET_FPS << " FPS\n\n";
    
    auto lastFrameTime = std::chrono::steady_clock::now();
    
    while (!renderer.shouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        
        auto frameDelta = std::chrono::duration_cast<std::chrono::microseconds>(
            now - lastFrameTime
        ).count();
        float deltaTime = frameDelta / 1000000.0f; 
        lastFrameTime = now;
        
        renderer.handleEvents();
        
        auto loadElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastLoadTime
        ).count();
        
        if (loadElapsed >= LOAD_INTERVAL_MS) {
            manager.loadVehiclesFromFiles();
            lastLoadTime = now;
        }
        
        auto spawnElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastSpawnTime
        ).count();
        
        if (spawnElapsed >= SPAWN_INTERVAL_MS) {
            manager.spawnQueuedVehicles();
            lastSpawnTime = now;
        }
        
        manager.updateVehiclePositions(deltaTime);
        
        auto cycleElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastCycleTime
        ).count();
        
        if (cycleElapsed >= CYCLE_INTERVAL_MS) {
            cycleCount++;
            
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << "Cycle #" << cycleCount << " - Switching lights...\n";
            
            manager.processCycle();
            manager.display();
            
            lastCycleTime = now;
        }
        
        manager.cleanupInactiveVehicles();
        
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
    
    std::cout << "\n\n🏁 Simulation ended. Total vehicles processed: " 
              << manager.getTotalProcessed() << std::endl;
    
    return 0;
}