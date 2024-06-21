#pragma once

#include "SDL.h"
#include "Sprite.h"
#include "Transparency.h"
#include <stdio.h>
#include <iostream>


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
    Sprite* pony = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    enum errors;

    enum animation_names {
        STANCE1,
        KICK,
        WALK,
        LIFTOFF,
        LANDING,
        STRETCH,
        HOVER,
        POINT,
        CRY,
        STANCE2,
        FLY,
        BLINK,
        CONFUSED,
        TIRED1,
        TIRED2,
        STAND,
        HAND,
        SLEEP
    };
};