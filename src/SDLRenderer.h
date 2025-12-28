#ifndef SDLRENDERER_H
#define SDLRENDERER_H
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <string>

class SDLRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int windowWidth;
    int windowHeight;
    bool isRunning;
    
  
    void drawSingleLight(int x, int y, bool isGreen, int radius, int glowRadius);
    void drawTrafficLightBox(int x, int y, bool isGreen);  
    void drawLaneLabels();
    void drawDigit(int digit, int x, int y, int size);
    
public:
    SDLRenderer(int width = 900, int height = 700);
    ~SDLRenderer();
    
    bool init(const std::string& title);
    void clear();
    void present();
    void handleEvents();
    bool shouldClose() const;
    
    void drawRoad();
    void drawTrafficLight(char currentLane, bool isPriority);
    void drawQueue(char lane, int queueSize);
    void drawStats(int cycle, int totalProcessed, int priorityCount);
    void drawVehicle(int x, int y, char lane);
    void drawNumber(int number, int x, int y, int size);
    
    void setColor(int r, int g, int b, int a = 255);
    void drawCircle(int centerX, int centerY, int radius);
    void drawRect(int x, int y, int w, int h, bool filled = true);
    void drawLine(int x1, int y1, int x2, int y2);
};

#endif