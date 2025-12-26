#include "SDLRenderer.h"
#include <iostream>
#include <cmath>

SDLRenderer::SDLRenderer(int width, int height) 
    : window(nullptr), renderer(nullptr), 
      windowWidth(width), windowHeight(height), isRunning(true) {}

SDLRenderer::~SDLRenderer() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool SDLRenderer::init(const std::string& title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void SDLRenderer::clear() {
    setColor(30, 30, 40);
    SDL_RenderClear(renderer);
}

void SDLRenderer::present() {
    SDL_RenderPresent(renderer);
}

void SDLRenderer::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

bool SDLRenderer::shouldClose() const {
    return !isRunning;
}

void SDLRenderer::setColor(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void SDLRenderer::drawRect(int x, int y, int w, int h, bool filled) {
    SDL_Rect rect = {x, y, w, h};
    if (filled) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void SDLRenderer::drawCircle(int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void SDLRenderer::drawRoad() {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int roadWidth = 80;
    
  
    setColor(60, 60, 70);
    drawRect(0, centerY - roadWidth/2, windowWidth, roadWidth);
    
 
    drawRect(centerX - roadWidth/2, 0, roadWidth, windowHeight);

   
    setColor(255, 255, 0);
    
    for (int i = 0; i < windowWidth; i += 20) {
        drawRect(i, centerY - 2, 10, 4);
    }

    for (int i = 0; i < windowHeight; i += 20) {
        drawRect(centerX - 2, i, 4, 10);
    }
}

void SDLRenderer::drawTrafficLight(char currentLane, bool isPriority) {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int lightRadius = 15;
    int offset = 60;
    
    
    int lightX_A = centerX - offset - 40;
    int lightY_A = centerY;
    if (currentLane == 'A') {
        setColor(0, 255, 0);
    } else {
        setColor(255, 0, 0);
    }
    drawCircle(lightX_A, lightY_A, lightRadius);
    
      int lightX_B = centerX;
    int lightY_B = centerY - offset - 40;
    if (currentLane == 'B') {
        setColor(0, 255, 0);
    } else {
        setColor(255, 0, 0);
    }
    drawCircle(lightX_B, lightY_B, lightRadius);
    
    
    int lightX_C = centerX + offset + 40;
    int lightY_C = centerY;
    if (currentLane == 'C') {
        setColor(0, 255, 0);
    } else {
        setColor(255, 0, 0);
    }
    drawCircle(lightX_C, lightY_C, lightRadius);
    

    int lightX_D = centerX;
    int lightY_D = centerY + offset + 40;
    if (currentLane == 'D') {
        setColor(0, 255, 0);
    } else {
        setColor(255, 0, 0);
    }
    drawCircle(lightX_D, lightY_D, lightRadius);
    
   
    if (isPriority) {
        setColor(255, 165, 0);
        drawCircle(centerX, centerY, 25);
        setColor(255, 255, 0);
        drawCircle(centerX, centerY, 20);
    }
}

void SDLRenderer::drawQueue(char lane, int queueSize) {
    int startX, startY;
    int spacing = 15;
    int carWidth = 12;
    int carHeight = 20;
    
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    
    setColor(70, 130, 180);
    
    switch(lane) {
        case 'A':
            startX = 50;
            startY = centerY - 10;
            for (int i = 0; i < queueSize && i < 15; i++) {
                drawRect(startX + i * spacing, startY, carWidth, carHeight);
            }
            break;
            
        case 'B':
            startX = centerX - 10;
            startY = 50;
            for (int i = 0; i < queueSize && i < 15; i++) {
                drawRect(startX, startY + i * spacing, carHeight, carWidth);
            }
            break;
            
        case 'C':
            startX = windowWidth - 50 - carWidth;
            startY = centerY - 10;
            for (int i = 0; i < queueSize && i < 15; i++) {
                drawRect(startX - i * spacing, startY, carWidth, carHeight);
            }
            break;
            
        case 'D':
            startX = centerX - 10;
            startY = windowHeight - 50 - carWidth;
            for (int i = 0; i < queueSize && i < 15; i++) {
                drawRect(startX, startY - i * spacing, carHeight, carWidth);
            }
            break;
    }
}

void SDLRenderer::drawStats(int cycle, int totalProcessed, int priorityCount) {
    setColor(20, 20, 30, 200);
    drawRect(10, 10, 250, 120);
    
    setColor(255, 255, 255);
    drawRect(10, 10, 250, 120, false);
    
    setColor(0, 255, 0);
    int barWidth = (cycle % 50) * 4;
    drawRect(20, 100, barWidth, 20);
}

void SDLRenderer::drawVehicle(int x, int y, char lane) {
    setColor(70, 130, 180);
    drawRect(x, y, 12, 20);
}