#include "DashWindow.h"
#include <iostream>

int main(int argc, char* argv[])
{

    DashWindow* dw = new DashWindow();
    dw->init();

    while (dw->running()) {
        dw->handleEvents();
        dw->update();
        dw->render();
    }

    dw->clean();

    return 0;
}