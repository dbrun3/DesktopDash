#include "DashWindow.h"

DashWindow::DashWindow() {
    //TODO
}

DashWindow::~DashWindow() {
    //TODO
}

void getWorkAreaSize(int* width, int* height) {
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    *width = workArea.right - workArea.left;
    *height = workArea.bottom - workArea.top;
}

void DashWindow::init() {
    int width, height;

    // initialize window
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized..." << std::endl;

        window = SDL_CreateWindow("SDL2 Window",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SIZE, SIZE, SDL_WINDOW_ALWAYS_ON_TOP);
        SDL_SetWindowBordered(window, SDL_FALSE);
        Transparency::enable_transparency(window);

        if (window) {
            std::cout << "Window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Start screen renderer created!" << std::endl;
        }
    }
    else {
        std::cout << "Could not create window. Returning..." << std::endl;
        isRunning = false;
        return;
    }

    // initialize sprite
    Sprite* sprite = new Sprite("assets/spritesheet.png", 6, 3, 3, SIZE, renderer);
    if (sprite->status() != sprite->OK) {
        std::cout << "Could not create pony. Code (" << sprite->status() << ") Returning..." << std::endl;
        isRunning = false;
        return;
    }

    // create pony with sprite and given play area
    getWorkAreaSize(&width, &height);
    pony = new Pony(sprite, width, height);

    isRunning = true;

    std::cout << "Pony made!" << std::endl;

}

void DashWindow::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT) {
            pony->pressed();
        }
    default:
        break;
    }
}

void DashWindow::update() {
    pony->update();
    SDL_SetWindowPosition(window, pony->getPosition().first, pony->getPosition().second);
}

void DashWindow::render() {
    SDL_RenderClear(renderer);
    Transparency::draw_transparent_background(renderer);
    pony->draw();

    SDL_RenderPresent(renderer);
}

void DashWindow::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Start screen cleaned." << std::endl;
}