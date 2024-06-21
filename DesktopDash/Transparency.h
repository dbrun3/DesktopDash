#pragma once
#ifndef _WIN32
#error "This code only works on Windows"
#endif

#include "SDL_syswm.h"
#include <Windows.h>

/*
* Transparent background provided by:
* https://filipsjanis.com/articles/transparent-window-background.html#:~:text=Setting%20the%20transparency%20attribute&text=You%20can%20set%20the%20attribute,use%20the%20SetWindowLong()%20function.
*
*/

class Transparency {

private:

    static HWND window_handle(SDL_Window* window) {
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(window, &wmInfo);
        HWND hwnd = wmInfo.info.win.window;
        return hwnd;
    }

    // Any pixel of this color will be rendered as transparent
    static const COLORREF transparent_colorref = RGB(255, 0, 255);

public:

    /**
     * Fills the entire renderer with transparent background.
     */
    static void draw_transparent_background(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, GetRValue(transparent_colorref), GetGValue(transparent_colorref), GetBValue(transparent_colorref), SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, NULL);
    }

    /**
     * Returns 0 on failure, 1 on success. Sets up the given window for rendering transparent pixels.
     */
    static int enable_transparency(SDL_Window* window) {
        HWND handle = window_handle(window);
        if (!SetWindowLong(handle, GWL_EXSTYLE, GetWindowLong(handle, GWL_EXSTYLE) | WS_EX_LAYERED)) {
            return 0;
        }
        if (!SetLayeredWindowAttributes(handle, transparent_colorref, 0, 1)) {
            return 0;
        }
        return 1;
    }
};