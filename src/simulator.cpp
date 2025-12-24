#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "TrafficManager.h"
#include "Vehicle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int ROAD_WIDTH = 120;
const int LANE_WIDTH = 40;

class SDLTrafficSimulator {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TrafficManager manager;
    int cycleCount;
    bool running;

    SDL_Color COLOR_ROAD = { 60, 60, 60, 255 };
    SDL_Color COLOR_GRASS = { 34, 139, 34, 255 };
    SDL_Color COLOR_LINE = { 255, 255, 255, 255 };
    SDL_Color COLOR_RED = { 255, 0, 0, 255 };
    SDL_Color COLOR_GREEN = { 0, 255, 0, 255 };
    SDL_Color COLOR_VEHICLE = { 0, 100, 255, 255 };
    SDL_Color COLOR_TEXT = { 255, 255, 255, 255 };

    void readVehiclesFromFile(const std::string& filename, char lane) {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
               
                std::string plate = line.substr(0, commaPos); 
                char laneName = line[commaPos + 1];
                manager.addVehicle(Vehicle(plate, laneName));
            }
        }
        file.close();
        std::ofstream clearFile(filename, std::ios::trunc);
        clearFile.close();
    }

    
    void pollAllLanes() {
        readVehiclesFromFile("lane_A.txt", 'A');
        readVehiclesFromFile("lane_B.txt", 'B');
        readVehiclesFromFile("lane_C.txt", 'C');
        readVehiclesFromFile("lane_D.txt", 'D');
    }

    
    void drawRectangle(float x, float y, float w, float h, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = { x, y, w, h };
        SDL_RenderFillRect(renderer, &rect);
    }

    void drawCircle(float centerX, float centerY, float radius, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        for (float w = 0; w < radius * 2; w++) {
            for (float h = 0; h < radius * 2; h++) {
                float dx = radius - w;
                float dy = radius - h;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }

    void drawText(const std::string& text, float x, float y, SDL_Color color) {
        if (!font) return;
       
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), color);
        if (!surface) return;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            SDL_FRect destRect = { x, y, (float)surface->w, (float)surface->h };
            SDL_RenderTexture(renderer, texture, nullptr, &destRect);
            SDL_DestroyTexture(texture);
        }
        SDL_DestroySurface(surface);
    }

    void drawJunction() {
        drawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_GRASS);
        float centerX = SCREEN_WIDTH / 2.0f;
        float centerY = SCREEN_HEIGHT / 2.0f;

        drawRectangle(0, centerY - ROAD_WIDTH / 2.0f, SCREEN_WIDTH, (float)ROAD_WIDTH, COLOR_ROAD);
        drawRectangle(centerX - ROAD_WIDTH / 2.0f, 0, (float)ROAD_WIDTH, SCREEN_HEIGHT, COLOR_ROAD);

        SDL_SetRenderDrawColor(renderer, COLOR_LINE.r, COLOR_LINE.g, COLOR_LINE.b, COLOR_LINE.a);
        for (int i = 1; i < 3; i++) {
            float y = centerY - ROAD_WIDTH / 2.0f + i * LANE_WIDTH;
            for (float x = 0; x < SCREEN_WIDTH; x += 20) {
                SDL_RenderLine(renderer, x, y, x + 10, y);
            }
        }
        for (int i = 1; i < 3; i++) {
            float x = centerX - ROAD_WIDTH / 2.0f + i * LANE_WIDTH;
            for (float y = 0; y < SCREEN_HEIGHT; y += 20) {
                SDL_RenderLine(renderer, x, y, x, y + 10);
            }
        }

        drawRectangle(centerX - ROAD_WIDTH / 2.0f, centerY - ROAD_WIDTH / 2.0f, (float)ROAD_WIDTH, (float)ROAD_WIDTH, { 100, 100, 100, 255 });
    }

    void drawTrafficLight(char lane, bool isGreen, float x, float y) {
        drawRectangle(x - 5, y, 10, 60, { 50, 50, 50, 255 });
        drawRectangle(x - 20, y - 50, 40, 50, { 40, 40, 40, 255 });

        if (!isGreen) drawCircle(x, y - 35, 12, COLOR_RED);
        else drawCircle(x, y - 35, 12, { 100, 0, 0, 255 });

        if (isGreen) drawCircle(x, y - 15, 12, COLOR_GREEN);
        else drawCircle(x, y - 15, 12, { 0, 100, 0, 255 });

        std::string label = "Lane ";
        label += lane;
        drawText(label, x - 20, y + 65, COLOR_TEXT);
    }

    void drawVehicleQueue(char lane, int count, float x, float y, bool horizontal) {
        float vehicleSize = 15.0f;
        float spacing = 20.0f;
        for (int i = 0; i < count && i < 10; i++) {
            if (horizontal) drawRectangle(x + i * spacing, y, vehicleSize, vehicleSize, COLOR_VEHICLE);
            else drawRectangle(x, y + i * spacing, vehicleSize, vehicleSize, COLOR_VEHICLE);
        }
        drawText(std::to_string(count), horizontal ? x : x + 20, horizontal ? y + 20 : y, COLOR_TEXT);
    }

    void drawUI() {
        float centerX = SCREEN_WIDTH / 2.0f;
        float centerY = SCREEN_HEIGHT / 2.0f;
      char currentLane = manager.getCurrentLane(); 

        drawTrafficLight('A', currentLane == 'A', centerX - 150, centerY - 150);
        drawTrafficLight('B', currentLane == 'B', centerX + 150, centerY - 150);
        drawTrafficLight('C', currentLane == 'C', centerX + 150, centerY + 150);
        drawTrafficLight('D', currentLane == 'D', centerX - 150, centerY + 150);

       
        drawVehicleQueue('A', manager.getLaneSize('A'), centerX - 100, centerY - 250, true);
        drawVehicleQueue('B', manager.getLaneSize('B'), centerX + 200, centerY - 100, false);
        drawVehicleQueue('C', manager.getLaneSize('C'), centerX + 100, centerY + 200, true);
        drawVehicleQueue('D', manager.getLaneSize('D'), centerX - 250, centerY + 100, false);

        drawRectangle(10, 10, 300, 150, { 0, 0, 0, 180 });
        drawText("Traffic Management System", 20, 20, COLOR_TEXT);
        drawText("Cycle: " + std::to_string(cycleCount), 20, 45, COLOR_TEXT);
    }

public:
    SDLTrafficSimulator() : window(nullptr), renderer(nullptr), font(nullptr), cycleCount(0), running(true) {}

    bool initialize() {
        if (!SDL_Init(SDL_INIT_VIDEO)) return false;
        if (!TTF_Init()) return false;

        window = SDL_CreateWindow("Traffic Management System", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (!window) return false;

        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) return false;

        font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 16);
        return true;
    }

    void run() {
        Uint64 lastUpdate = SDL_GetTicks();
        const Uint64 UPDATE_INTERVAL = 3000;

        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) running = false;
            }

            Uint64 currentTime = SDL_GetTicks();
            if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
                cycleCount++;
                pollAllLanes(); // FIX: Corrected name
                manager.processCycle();
                lastUpdate = currentTime;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            drawJunction(); // FIX: Corrected typo
            drawUI();
            SDL_RenderPresent(renderer);
        }
    }

    void cleanup() {
        if (font) TTF_CloseFont(font);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
};

int main(int argc, char* argv[]) {
    SDLTrafficSimulator simulator;
    if (!simulator.initialize()) return 1;
    simulator.run();
    simulator.cleanup();
    return 0;
}