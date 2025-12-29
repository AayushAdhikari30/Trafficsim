#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include <SDL2/SDL.h>
#include <string>

class SDLRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int windowWidth;
    int windowHeight;
    bool isRunning;
    
    void drawDigit(int digit, int x, int y, int size);
    void drawNumber(int number, int x, int y, int size);
    void drawLaneLabels();
    void drawTrafficLightBox(int x, int y, bool isGreen);
    
public:
    SDLRenderer(int width, int height);
    ~SDLRenderer();
    
    bool init(const std::string& title);
    void clear();
    void present();
    void handleEvents();
    bool shouldClose() const;
    
    void setColor(int r, int g, int b, int a = 255);
    void drawRect(int x, int y, int w, int h, bool filled = true);
    void drawCircle(int centerX, int centerY, int radius);
    void drawLine(int x1, int y1, int x2, int y2);
    
    void drawRoad();
    void drawTrafficLight(char currentLane, bool isPriority);
    void drawAnimatedVehicle(float x, float y, char road, int lane);
    void drawQueue(char lane, int queueSize);
    void drawStats(int cycle, int totalProcessed, int priorityCount);
    void drawVehicle(int x, int y, char lane);
    void drawSingleLight(int x, int y, bool isGreen, int radius = 12, int glowRadius = 15);
};

#endif