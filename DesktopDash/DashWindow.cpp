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

// Function to get the main window handle of the current process
HWND GetCurrentProcessMainWindow() {
    HWND hwnd = NULL;
    DWORD currentProcessId = GetCurrentProcessId();

    // Enumerate all windows to find the main window of the current process
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        DWORD windowProcessId;
        GetWindowThreadProcessId(hwnd, &windowProcessId);
        if (windowProcessId == static_cast<DWORD>(lParam)) {
            *(reinterpret_cast<HWND*>(lParam)) = hwnd;
            return FALSE; // Stop enumeration
        }
        return TRUE; // Continue enumeration
        }, reinterpret_cast<LPARAM>(&currentProcessId));

    return hwnd;
}

bool IsWindowFullscreen(HWND hwnd) {
    // Get the dimensions of the primary monitor
    HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfo(hMonitor, &monitorInfo)) {
        // Get the dimensions of the window
        RECT windowRect;
        if (GetWindowRect(hwnd, &windowRect)) {
            // Compare window dimensions with monitor dimensions
            if (windowRect.left == monitorInfo.rcMonitor.left &&
                windowRect.top == monitorInfo.rcMonitor.top &&
                windowRect.right == monitorInfo.rcMonitor.right &&
                windowRect.bottom == monitorInfo.rcMonitor.bottom) {
                return true;
            }
        }
    }
    return false;
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
    self = GetCurrentProcessMainWindow();

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

    // SDL events
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

    //Windows: Detect new window event
    HWND hwnd = GetForegroundWindow();
    if (hwnd == NULL || IsWindowFullscreen(hwnd) || hwnd != self) {
        pony->fullscreen_mode();
        return;
    }

    //If not fullscreen, get the window's position and dimensions
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        int width = rect.right - rect.left;
        int wx = rect.left;
        int wy = rect.top;
        pony->window_mode(wx, wy, width);
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