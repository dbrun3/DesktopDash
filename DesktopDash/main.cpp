#include "DashWindow.h"
#include <iostream>

int main(int argc, char* argv[])
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    int frameTime;


    DashWindow* dw = new DashWindow();
    dw->init();

    while (dw->running()) {

        Uint32 frameStart = SDL_GetTicks();

        dw->handleEvents();
        dw->update();
        dw->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    dw->clean();

    return 0;
}