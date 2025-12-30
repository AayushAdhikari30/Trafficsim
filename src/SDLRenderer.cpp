#include "SDLRenderer.h"
#include <iostream>
#include <cmath>
#include <string>

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
    setColor(128, 128, 128);  
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

void SDLRenderer::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void SDLRenderer::drawDigit(int digit, int x, int y, int size) {
    bool segments[10][7] = {
        {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1},
        {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}
    };
    
    if (digit < 0 || digit > 9) return;
    
    int w = size;
    int h = size / 2;
    
    if (segments[digit][0]) drawRect(x, y, w, 2);
    if (segments[digit][1]) drawRect(x + w - 2, y, 2, h);
    if (segments[digit][2]) drawRect(x + w - 2, y + h, 2, h);
    if (segments[digit][3]) drawRect(x, y + h * 2 - 2, w, 2);
    if (segments[digit][4]) drawRect(x, y + h, 2, h);
    if (segments[digit][5]) drawRect(x, y, 2, h);
    if (segments[digit][6]) drawRect(x, y + h - 1, w, 2);
}

void SDLRenderer::drawNumber(int number, int x, int y, int size) {
    if (number == 0) {
        drawDigit(0, x, y, size);
        return;
    }
    
    std::string numStr = std::to_string(number);
    int spacing = size + 4;
    
    for (size_t i = 0; i < numStr.length(); i++) {
        int digit = numStr[i] - '0';
        drawDigit(digit, x + i * spacing, y, size);
    }
}

void SDLRenderer::drawRoad() {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int roadWidth = 180;
    int laneWidth = 60;
    
    setColor(50, 50, 50);
    drawRect(0, centerY - roadWidth/2, windowWidth, roadWidth);
    drawRect(centerX - roadWidth/2, 0, roadWidth, windowHeight);
    
    setColor(255, 255, 255);
    
    for (int x = 10; x < centerX - roadWidth/2 - 10; x += 30) {
        drawRect(x, centerY - roadWidth/2 + laneWidth - 2, 20, 4);
        drawRect(x, centerY - roadWidth/2 + 2*laneWidth - 2, 20, 4);
    }
    
    for (int x = centerX + roadWidth/2 + 10; x < windowWidth - 10; x += 30) {
        drawRect(x, centerY - roadWidth/2 + laneWidth - 2, 20, 4);
        drawRect(x, centerY - roadWidth/2 + 2*laneWidth - 2, 20, 4);
    }
    
    for (int y = 10; y < centerY - roadWidth/2 - 10; y += 30) {
        drawRect(centerX - roadWidth/2 + laneWidth - 2, y, 4, 20);
        drawRect(centerX - roadWidth/2 + 2*laneWidth - 2, y, 4, 20);
    }
    
    for (int y = centerY + roadWidth/2 + 10; y < windowHeight - 10; y += 30) {
        drawRect(centerX - roadWidth/2 + laneWidth - 2, y, 4, 20);
        drawRect(centerX - roadWidth/2 + 2*laneWidth - 2, y, 4, 20);
    }
    
    setColor(55, 55, 55);
    drawRect(centerX - roadWidth/2, centerY - roadWidth/2, roadWidth, roadWidth);
    
    setColor(255, 255, 255);
    int stripWidth = 8;
    int stripGap = 4;
    int stopDistance = roadWidth/2 + 8;
    
    for (int i = 0; i < roadWidth; i += stripWidth + stripGap) {
        drawRect(centerX - stopDistance, centerY - roadWidth/2 + i, 6, stripWidth);
        drawRect(centerX + stopDistance - 6, centerY - roadWidth/2 + i, 6, stripWidth);
        drawRect(centerX - roadWidth/2 + i, centerY - stopDistance, stripWidth, 6);
        drawRect(centerX - roadWidth/2 + i, centerY + stopDistance - 6, stripWidth, 6);
    }
    
    setColor(220, 180, 0);
    drawRect(0, centerY - roadWidth/2 - 2, windowWidth, 2);
    drawRect(0, centerY + roadWidth/2, windowWidth, 2);
    drawRect(centerX - roadWidth/2 - 2, 0, 2, windowHeight);
    drawRect(centerX + roadWidth/2, 0, 2, windowHeight);
    
    drawLaneLabels();
}

void SDLRenderer::drawLaneLabels() {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int roadWidth = 180;
    int laneWidth = 60;
    
    setColor(255, 255, 150);
    
    int labelX = 25;
    drawNumber(1, labelX, centerY - roadWidth/2 + 20, 10);
    drawNumber(2, labelX, centerY - roadWidth/2 + laneWidth + 20, 10);
    drawNumber(3, labelX, centerY - roadWidth/2 + 2*laneWidth + 20, 10);
    
    int labelY = 25;
    drawNumber(1, centerX - roadWidth/2 + 20, labelY, 10);
    drawNumber(2, centerX - roadWidth/2 + laneWidth + 20, labelY, 10);
    drawNumber(3, centerX - roadWidth/2 + 2*laneWidth + 20, labelY, 10);
    
    labelX = windowWidth - 40;
    drawNumber(1, labelX, centerY - roadWidth/2 + 20, 10);
    drawNumber(2, labelX, centerY - roadWidth/2 + laneWidth + 20, 10);
    drawNumber(3, labelX, centerY - roadWidth/2 + 2*laneWidth + 20, 10);
    
    labelY = windowHeight - 40;
    drawNumber(1, centerX - roadWidth/2 + 20, labelY, 10);
    drawNumber(2, centerX - roadWidth/2 + laneWidth + 20, labelY, 10);
    drawNumber(3, centerX - roadWidth/2 + 2*laneWidth + 20, labelY, 10);
}

void SDLRenderer::drawTrafficLight(char currentLane, bool isPriority) {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int offset = 120;
    
    drawTrafficLightBox(centerX - offset, centerY, currentLane == 'A');
    drawTrafficLightBox(centerX, centerY - offset, currentLane == 'B');
    drawTrafficLightBox(centerX + offset, centerY, currentLane == 'C');
    drawTrafficLightBox(centerX, centerY + offset, currentLane == 'D');
    
    if (isPriority) {
        setColor(255, 165, 0);
        drawCircle(centerX, centerY, 30);
        setColor(255, 200, 100);
        drawNumber(1, centerX - 6, centerY - 10, 12);
    }
}

void SDLRenderer::drawTrafficLightBox(int x, int y, bool isGreen) {
    setColor(30, 30, 30);
    drawRect(x - 20, y - 30, 40, 60);
    
    setColor(60, 60, 60);
    drawRect(x - 20, y - 30, 40, 60, false);
    
    int lightRadius = 12;
    
    if (isGreen) {
        setColor(0, 200, 0);
        drawCircle(x, y, lightRadius);
        setColor(100, 255, 100);
        drawCircle(x, y, lightRadius - 4);
        setColor(80, 0, 0);
        drawCircle(x, y - 20, lightRadius - 2);
    } else {
        setColor(200, 0, 0);
        drawCircle(x, y - 20, lightRadius);
        setColor(255, 100, 100);
        drawCircle(x, y - 20, lightRadius - 4);
        setColor(0, 80, 0);
        drawCircle(x, y, lightRadius - 2);
    }
}

void SDLRenderer::drawAnimatedVehicle(float x, float y, char road, int lane) {
    int carWidth = 20;
    int carHeight = 35;
    
    // Determine car color based on road
    int bodyR, bodyG, bodyB;
    if (road == 'A') {
        bodyR = 0; bodyG = 120; bodyB = 255;  // Blue
    } else if (road == 'B') {
        bodyR = 0; bodyG = 200; bodyB = 50;   // Green
    } else if (road == 'C') {
        bodyR = 255; bodyG = 140; bodyB = 0;  // Orange
    } else {
        bodyR = 220; bodyG = 20; bodyB = 220; // Purple
    }
    
    // Draw car body (rounded rectangle approximation)
    setColor(bodyR, bodyG, bodyB);
    drawRect((int)x + 2, (int)y, carWidth - 4, carHeight);  // Main body
    
    // Draw car roof/windows
    setColor(bodyR + 50, bodyG + 50, bodyB + 50);  // Lighter shade
    drawRect((int)x + 4, (int)y + 3, carWidth - 8, 12);  // Roof
    
    // Draw windows
    setColor(200, 220, 255);  // Light blue for windows
    drawRect((int)x + 5, (int)y + 4, carWidth - 10, 4);  // Front window
    drawRect((int)x + 5, (int)y + 10, carWidth - 10, 4); // Rear window
    
    // Draw wheels
    setColor(30, 30, 30);  // Dark gray for tires
    drawCircle((int)x + 5, (int)y + carHeight - 3, 3);   // Front wheel
    drawCircle((int)x + carWidth - 5, (int)y + carHeight - 3, 3); // Rear wheel
    
    // Draw wheel rims
    setColor(150, 150, 150);  // Light gray for rims
    drawCircle((int)x + 5, (int)y + carHeight - 3, 1);   // Front rim
    drawCircle((int)x + carWidth - 5, (int)y + carHeight - 3, 1); // Rear rim
    
    // Draw headlights/taillights
    if (road == 'A' || road == 'C') {  // Horizontal roads
        setColor(255, 255, 200);  // Yellow for headlights
        drawRect((int)x + carWidth - 2, (int)y + 5, 2, 3);  // Front light
        setColor(255, 0, 0);  // Red for taillights
        drawRect((int)x, (int)y + 5, 2, 3);  // Rear light
    } else {  // Vertical roads
        setColor(255, 255, 200);  // Yellow for headlights
        drawRect((int)x + 5, (int)y, 3, 2);  // Front light
        setColor(255, 0, 0);  // Red for taillights
        drawRect((int)x + 5, (int)y + carHeight - 2, 3, 2);  // Rear light
    }
}

void SDLRenderer::drawQueue(char lane, int queueSize) {
}

void SDLRenderer::drawStats(int cycle, int totalProcessed, int priorityCount) {
}

void SDLRenderer::drawVehicle(int x, int y, char lane) {
    setColor(70, 130, 180);
    drawRect(x, y, 18, 32);
    setColor(100, 160, 200);
    drawRect(x + 3, y + 3, 12, 10);
}

void SDLRenderer::drawSingleLight(int x, int y, bool isGreen, int radius, int glowRadius) {
    drawTrafficLightBox(x, y, isGreen);
}