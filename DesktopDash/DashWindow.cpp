#include "DashWindow.h"

DashWindow::DashWindow() {
    //TODO
}

DashWindow::~DashWindow() {
    //TODO
}

void DashWindow::init() {
    int flags = SDL_WINDOW_ALWAYS_ON_TOP;

    // initialize window
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized..." << std::endl;

        window = SDL_CreateWindow("SDL2 Window",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            64, 64, flags);
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

    // initialize pony
    pony = new Sprite("assets/spritesheet.png", 6, 3, 3, 64, renderer);
    if (pony->status() != pony->OK) {
        std::cout << "Could not create pony. Code (" << pony->status() << ") Returning..." << std::endl;
        isRunning = false;
        return;
    }

    // test
    pony->play(WALK);

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
            //TODO
        }
    default:
        break;
    }
}

void DashWindow::update() {
    //TODO

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