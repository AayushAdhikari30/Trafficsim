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
    setColor(15, 20, 30);  // Dark background
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

// Draw simple digit (0-9) using segments
void SDLRenderer::drawDigit(int digit, int x, int y, int size) {
    // 7-segment display style
    // Segments: top, top-right, bottom-right, bottom, bottom-left, top-left, middle
    bool segments[10][7] = {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };
    
    if (digit < 0 || digit > 9) return;
    
    int w = size;
    int h = size / 2;
    
    // Top
    if (segments[digit][0]) drawRect(x, y, w, 2);
    // Top-right
    if (segments[digit][1]) drawRect(x + w - 2, y, 2, h);
    // Bottom-right
    if (segments[digit][2]) drawRect(x + w - 2, y + h, 2, h);
    // Bottom
    if (segments[digit][3]) drawRect(x, y + h * 2 - 2, w, 2);
    // Bottom-left
    if (segments[digit][4]) drawRect(x, y + h, 2, h);
    // Top-left
    if (segments[digit][5]) drawRect(x, y, 2, h);
    // Middle
    if (segments[digit][6]) drawRect(x, y + h - 1, w, 2);
}

// Draw a number (multiple digits)
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

// Enhanced road with 3 visible lanes
void SDLRenderer::drawRoad() {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int roadWidth = 150;  // Wider to clearly show 3 lanes
    int laneWidth = roadWidth / 3;  // 50 pixels per lane
    
    // Draw main roads (asphalt)
    setColor(45, 45, 55);
    // Horizontal road
    drawRect(0, centerY - roadWidth/2, windowWidth, roadWidth);
    // Vertical road
    drawRect(centerX - roadWidth/2, 0, roadWidth, windowHeight);
    
    // Draw lane dividers - HORIZONTAL ROAD
    setColor(255, 255, 100);  // Yellow
    
    // West side (Road A approach)
    for (int x = 10; x < centerX - roadWidth/2; x += 25) {
        // Divider between L1 and L2
        drawRect(x, centerY - roadWidth/2 + laneWidth, 15, 3);
        // Divider between L2 and L3
        drawRect(x, centerY - roadWidth/2 + 2*laneWidth, 15, 3);
    }
    
    // East side (Road C approach)
    for (int x = centerX + roadWidth/2; x < windowWidth - 10; x += 25) {
        drawRect(x, centerY - roadWidth/2 + laneWidth, 15, 3);
        drawRect(x, centerY - roadWidth/2 + 2*laneWidth, 15, 3);
    }
    
    // Draw lane dividers - VERTICAL ROAD
    // North side (Road B approach)
    for (int y = 10; y < centerY - roadWidth/2; y += 25) {
        drawRect(centerX - roadWidth/2 + laneWidth, y, 3, 15);
        drawRect(centerX - roadWidth/2 + 2*laneWidth, y, 3, 15);
    }
    
    // South side (Road D approach)
    for (int y = centerY + roadWidth/2; y < windowHeight - 10; y += 25) {
        drawRect(centerX - roadWidth/2 + laneWidth, y, 3, 15);
        drawRect(centerX - roadWidth/2 + 2*laneWidth, y, 3, 15);
    }
    
    // Draw intersection center (clear area)
    setColor(40, 40, 50);
    drawRect(centerX - roadWidth/2, centerY - roadWidth/2, roadWidth, roadWidth);
    
    // Draw stop lines at intersection
    setColor(255, 255, 255);
    int stopOffset = roadWidth/2 + 5;
    // West stop line
    drawRect(centerX - stopOffset, centerY - roadWidth/2, 4, roadWidth);
    // East stop line
    drawRect(centerX + stopOffset - 4, centerY - roadWidth/2, 4, roadWidth);
    // North stop line
    drawRect(centerX - roadWidth/2, centerY - stopOffset, roadWidth, 4);
    // South stop line
    drawRect(centerX - roadWidth/2, centerY + stopOffset - 4, roadWidth, 4);
    
    // Draw road edges
    setColor(180, 180, 180);
    // Horizontal edges
    drawRect(0, centerY - roadWidth/2 - 3, windowWidth, 3);
    drawRect(0, centerY + roadWidth/2, windowWidth, 3);
    // Vertical edges
    drawRect(centerX - roadWidth/2 - 3, 0, 3, windowHeight);
    drawRect(centerX + roadWidth/2, 0, 3, windowHeight);
    
    // Draw lane labels
    drawLaneLabels();
}

void SDLRenderer::drawLaneLabels() {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int roadWidth = 150;
    int laneWidth = roadWidth / 3;
    
    setColor(255, 255, 255);
    
    // Road A labels (West - Left side)
    int labelX_A = 30;
    int baseY = centerY - roadWidth/2;
    
    // L1 label
    drawRect(labelX_A, baseY + 10, 2, 15);
    drawRect(labelX_A, baseY + 25, 10, 2);
    
    // L2 label  
    drawRect(labelX_A, baseY + laneWidth + 10, 2, 15);
    drawRect(labelX_A, baseY + laneWidth + 25, 10, 2);
    drawDigit(2, labelX_A + 12, baseY + laneWidth + 10, 8);
    
    // L3 label
    drawRect(labelX_A, baseY + 2*laneWidth + 10, 2, 15);
    drawRect(labelX_A, baseY + 2*laneWidth + 25, 10, 2);
    
    // Road B labels (North - Top)
    int labelY_B = 30;
    int baseX = centerX - roadWidth/2;
    
    // Road C labels (East - Right side)
    int labelX_C = windowWidth - 40;
    
    // Road D labels (South - Bottom)
    int labelY_D = windowHeight - 40;
}

// Enhanced traffic lights with glow
void SDLRenderer::drawTrafficLight(char currentLane, bool isPriority) {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int lightRadius = 20;
    int glowRadius = 32;
    int offset = 100;
    
    // Road A - West
    int lightX_A = centerX - offset - 60;
    int lightY_A = centerY;
    drawSingleLight(lightX_A, lightY_A, currentLane == 'A', lightRadius, glowRadius);
    
    // Road B - North
    int lightX_B = centerX;
    int lightY_B = centerY - offset - 60;
    drawSingleLight(lightX_B, lightY_B, currentLane == 'B', lightRadius, glowRadius);
    
    // Road C - East
    int lightX_C = centerX + offset + 60;
    int lightY_C = centerY;
    drawSingleLight(lightX_C, lightY_C, currentLane == 'C', lightRadius, glowRadius);
    
    // Road D - South
    int lightX_D = centerX;
    int lightY_D = centerY + offset + 60;
    drawSingleLight(lightX_D, lightY_D, currentLane == 'D', lightRadius, glowRadius);
    
    // Priority indicator
    if (isPriority) {
        setColor(255, 100, 0, 150);
        drawCircle(centerX, centerY, 40);
        setColor(255, 150, 0);
        drawCircle(centerX, centerY, 35);
        setColor(255, 200, 50);
        drawCircle(centerX, centerY, 28);
        setColor(255, 255, 100);
        drawCircle(centerX, centerY, 20);
    }
}

void SDLRenderer::drawSingleLight(int x, int y, bool isGreen, int radius, int glowRadius) {
    if (isGreen) {
        // Green light with glow
        setColor(0, 255, 0, 80);
        drawCircle(x, y, glowRadius);
        setColor(0, 220, 0);
        drawCircle(x, y, radius);
        setColor(100, 255, 100);
        drawCircle(x, y, radius - 5);
        setColor(200, 255, 200);
        drawCircle(x, y, radius - 10);
    } else {
        // Red light (dim)
        setColor(180, 0, 0);
        drawCircle(x, y, radius);
        setColor(100, 0, 0);
        drawCircle(x, y, radius - 5);
    }
    
    // Light housing
    setColor(30, 30, 30);
    drawRect(x - 30, y - 40, 60, 80, false);
    drawRect(x - 28, y - 38, 56, 76, false);
}

// Enhanced vehicle queue with better cars
void SDLRenderer::drawQueue(char lane, int queueSize) {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int roadWidth = 150;
    int laneWidth = roadWidth / 3;
    
    int carWidth = 16;
    int carHeight = 28;
    int spacing = 20;
    
    // Road A - West (vehicles in Lane 2)
    if (lane == 'A') {
        int startX = 80;
        int startY = centerY - roadWidth/2 + laneWidth + (laneWidth - carHeight) / 2;
        
        setColor(70, 150, 220);  // Blue
        for (int i = 0; i < queueSize && i < 20; i++) {
            int x = startX + i * spacing;
            int y = startY;
            
            // Car body
            drawRect(x, y, carWidth, carHeight);
            // Windshield
            setColor(100, 180, 240);
            drawRect(x + 2, y + 2, carWidth - 4, 8);
            // Rear window
            drawRect(x + 2, y + carHeight - 10, carWidth - 4, 8);
            setColor(70, 150, 220);
        }
    }
    
    // Road B - North (vehicles in Lane 2)
    else if (lane == 'B') {
        int startX = centerX - roadWidth/2 + laneWidth + (laneWidth - carHeight) / 2;
        int startY = 80;
        
        setColor(100, 200, 100);  // Green
        for (int i = 0; i < queueSize && i < 20; i++) {
            int x = startX;
            int y = startY + i * spacing;
            
            drawRect(x, y, carHeight, carWidth);
            setColor(120, 220, 120);
            drawRect(x + 2, y + 2, 8, carWidth - 4);
            drawRect(x + carHeight - 10, y + 2, 8, carWidth - 4);
            setColor(100, 200, 100);
        }
    }
    
    // Road C - East (vehicles in Lane 2)
    else if (lane == 'C') {
        int startX = windowWidth - 80 - carWidth;
        int startY = centerY - roadWidth/2 + laneWidth + (laneWidth - carHeight) / 2;
        
        setColor(220, 150, 70);  // Orange
        for (int i = 0; i < queueSize && i < 20; i++) {
            int x = startX - i * spacing;
            int y = startY;
            
            drawRect(x, y, carWidth, carHeight);
            setColor(240, 180, 100);
            drawRect(x + 2, y + 2, carWidth - 4, 8);
            drawRect(x + 2, y + carHeight - 10, carWidth - 4, 8);
            setColor(220, 150, 70);
        }
    }
    
    // Road D - South (vehicles in Lane 2)
    else if (lane == 'D') {
        int startX = centerX - roadWidth/2 + laneWidth + (laneWidth - carHeight) / 2;
        int startY = windowHeight - 80 - carWidth;
        
        setColor(200, 100, 180);  // Purple
        for (int i = 0; i < queueSize && i < 20; i++) {
            int x = startX;
            int y = startY - i * spacing;
            
            drawRect(x, y, carHeight, carWidth);
            setColor(220, 120, 200);
            drawRect(x + 2, y + 2, 8, carWidth - 4);
            drawRect(x + carHeight - 10, y + 2, 8, carWidth - 4);
            setColor(200, 100, 180);
        }
    }
}

// Enhanced stats panel with numbers
void SDLRenderer::drawStats(int cycle, int totalProcessed, int priorityCount) {
    // Main panel background
    setColor(25, 30, 40, 240);
    drawRect(10, 10, 300, 180);
    
    // Border
    setColor(80, 100, 130);
    drawRect(10, 10, 300, 180, false);
    drawRect(11, 11, 298, 178, false);
    
    // Title bar
    setColor(40, 50, 70);
    drawRect(10, 10, 300, 35);
    setColor(100, 150, 200);
    drawRect(20, 18, 3, 18);
    drawRect(25, 18, 3, 18);
    
    // Labels and values
    setColor(150, 170, 200);
    
    // Cycle counter
    drawRect(20, 55, 8, 2);
    drawRect(20, 55, 2, 15);
    setColor(100, 255, 150);
    drawNumber(cycle, 80, 52, 12);
    
    // Total processed
    setColor(150, 170, 200);
    drawRect(20, 85, 8, 2);
    drawRect(20, 85, 2, 15);
    setColor(100, 200, 255);
    drawNumber(totalProcessed, 80, 82, 12);
    
    // Priority queue
    setColor(150, 170, 200);
    drawRect(20, 115, 8, 2);
    drawRect(20, 115, 2, 15);
    setColor(255, 180, 100);
    drawNumber(priorityCount, 80, 112, 12);
    
    // Progress bar for cycle
    setColor(30, 35, 45);
    drawRect(20, 150, 260, 20);
    setColor(80, 100, 130);
    drawRect(20, 150, 260, 20, false);
    
    setColor(100, 200, 150);
    int barWidth = (cycle % 10) * 26;
    if (barWidth > 260) barWidth = 260;
    drawRect(20, 150, barWidth, 20);
}

void SDLRenderer::drawVehicle(int x, int y, char lane) {
    setColor(70, 130, 180);
    drawRect(x, y, 14, 24);
    setColor(100, 160, 200);
    drawRect(x + 2, y + 2, 10, 8);
}