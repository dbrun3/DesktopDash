#pragma once

#include "Sprite.h"
#include <utility>


class Pony {

public:

	Pony(Sprite* s, int aX, int aY);
	~Pony();

    void window_mode(int x, int y, int w);
    void fullscreen_mode();

    void pressed();
    void update();
	void draw() { sprite->draw(); }

	// Position returned is where the top left of the window should be on the desktop for the pony's feet to be at (x,y)
	std::pair<int, int> getPosition() { return std::pair<int, int>(x - sprite->getSize() / 2, y - sprite->getSize()); }

private:
	int x, y = 0;
    int targetX, targetY = 0;
    int floorX, floorY, floorWidth = 0; 
    int areaX, areaY = 0;
    int timeAwake = 0;
    bool isSleeping = false;
    bool isStanding = false;
    bool isWalking = false;
    bool isWaking = false;
    bool isFlying = false;
    bool isFalling = false;

	Sprite* sprite;

    void sleep();
    void wake();
    void walk();

    enum animation_names {
        STANCE1,
        KICK,
        WALK,
        TAKEOFF,
        LANDING,
        STRETCH,
        HOVER,
        STAND,
        CRY,
        STANCE2,
        FLY,
        BLINK,
        CONFUSED,
        TIRED1,
        TIRED2,
        LIFTOFF,
        WAKE,
        SLEEP
    };
};