#pragma once

#include "SDL.h"
#include "Sprite.h"
#include "Pony.h"
#include "Transparency.h"
#include <stdio.h>
#include <iostream>
#include <utility>

#define SIZE 64

class DashWindow
{
public:
    DashWindow();
    ~DashWindow();

    void init();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }

private:

    bool isRunning = false;
    std::pair<int, int> lastWindow = std::pair<int, int>(0, 0);
    Pony* pony = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    HWND self;
};