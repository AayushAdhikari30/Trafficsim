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
    
public:
    SDLRenderer(int width = 800, int height = 600);
    ~SDLRenderer();
    
    bool init(const std::string& title);
    void clear();
    void present();
    void handleEvents();
    bool shouldClose() const;
    
    // Drawing functions
    void drawRoad();
    void drawTrafficLight(char currentLane, bool isPriority);
    void drawQueue(char lane, int queueSize);
    void drawStats(int cycle, int totalProcessed, int priorityCount);
    void drawVehicle(int x, int y, char lane);
    
    // Helper
    void setColor(int r, int g, int b, int a = 255);
    void drawCircle(int centerX, int centerY, int radius);
    void drawRect(int x, int y, int w, int h, bool filled = true);
    void drawText(const std::string& text, int x, int y);
};

#endif