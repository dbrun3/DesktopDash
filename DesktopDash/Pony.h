#pragma once

#include "Sprite.h"
#include <utility>
#include <bitset>
#include <stdlib.h>
#include <time.h>


class Pony {

public:

	Pony(Sprite* s, int aX, int aY);
	~Pony();

    void window_mode(int x, int y, int w);
    void fullscreen_mode();
    void pressed();
    void flyBy();

    void update();
	void draw() { sprite->draw(); }
    
    void setState(int s) { state.reset(); state.set(s, true); }
    int getState() { int r = 0; while (!state.test(r)) { r++; if (r == state.size()) return -1; } return r;}
    bool isFlyingBy() { return flyby; }

	// Position returned is where the top left of the window should be on the desktop for the pony's feet to be at (x,y)
	std::pair<int, int> getPosition() { return std::pair<int, int>(x - sprite->getSize() / 2, y - sprite->getSize()); }

private:
	int x, y = 0;
    int targetX, targetY = 0;
    int floorX, floorY, floorWidth = 0; 
    std::pair<int, int> prevFloor = std::pair<int, int>(0, 0);
    int areaX, areaY = 0;
    bool flyby;
    double timeAwake = 0;

    std::bitset<8> state;

	Sprite* sprite;

    enum states {
        SLEEPING,
        SLEEPY,
        WAKING,
        STANDING,
        WALKING,
        HOVERING,
        FLYING
    };
};