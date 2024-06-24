#include "Pony.h"

Pony::Pony(Sprite* s, int aX, int aY) {
	sprite = s;
	areaX = aX;
	areaY = aY;
	floorX = 0;
	floorY = areaY;
	floorWidth = areaX;

	x = 3 * floorWidth / 4;
	y = floorY + 8;

	sprite->play(sprite->SLEEP, 600);
	setState(SLEEPING);
}

Pony::~Pony() {
	//TODO
}

void Pony::fullscreen_mode() {
	if (flyby || (std::pair<int, int>(0, areaY) == prevFloor)) return;
	floorX = 0;
	floorY = areaY;
	floorWidth = areaX;
	prevFloor = std::pair<int, int>(floorX, floorY);


	int t = floorX + (floorWidth / 2);
	if (abs(x - t) > floorWidth / 2) { landingOffset = 0; return; }
	landingOffset = (1.0 - (abs(x - t) / t)) * (floorWidth / 2) * ((x > t)? -1 : 1);

}

void Pony::window_mode(int wx, int wy, int width) {
	if (flyby || (std::pair<int, int>(wx, wy) == prevFloor)) return;
	floorX = wx;
	floorY = wy;
	floorWidth = width;
	prevFloor = std::pair<int, int>(floorX, floorY);

	int t = floorX + (floorWidth / 2);
	if (abs(x - t) > floorWidth / 2) {landingOffset = 0; return;}
	landingOffset = (1.0 - (abs(x - t) / t)) * (floorWidth / 2) * ((x > t) ? -1 : 1);
}

void Pony::flyBy() {

	bool flip = !sprite->isFacingLeft();
	if (getState() == STANDING) {
		floorX = flip? (areaX + 30) : -60;
		floorY = y - 1;
		floorWidth = 30;
		flyby = flip? 1 : -1;
	}
}

// Wakey wakey
void Pony::pressed() {
	timeAwake = SDL_GetTicks();
	if (state[SLEEPING] || state[SLEEPY]) {
		if (state[SLEEPING]) y -= 8;
		setState(WAKING);
		return;
	}
	if (state[STANDING]) {
		sprite->play(sprite->SMILE, 100, 500);
		timeAwake -= 3000;
	}
}

// Update based on state
void Pony::update() {

	// Pony faces the mouse
	int mousex, mousey;
	SDL_GetGlobalMouseState(&mousex, &mousey);

	switch (getState()) {

	case SLEEPING:
		sprite->play(sprite->SLEEP, 800);
		if (y != floorY + 8 && y != areaY + 8) {
			setState(HOVERING);
			return;
		}
		break;
	case WAKING:
		sprite->play(sprite->WAKE, 150, 100);
		if (y != floorY && y != areaY) {
			setState(HOVERING);
			return;
		}
		break;
	case SLEEPY:
		sprite->play(sprite->TIRED2, 800, 10000);
		if (y != floorY && y != areaY) {
			setState(HOVERING);
			return;
		}
		break;

	case WALKING:
		sprite->play(sprite->WALK);
		timeAwake = SDL_GetTicks() - 3000;

		if (abs(targetX - x) < 5 || abs(mousex - x) < 5) {
			sprite->play(sprite->STAND);
			setState(STANDING);
			return;
		}
		if (targetX > x) {
			if (sprite->isFacingLeft()) sprite->flip();
			x += 1;
		}
		if (targetX < x) {
			if (!sprite->isFacingLeft()) sprite->flip();
			x -= 1;
		}
		if (y != floorY) {
			sprite->play(sprite->LIFTOFF, 100, 30);
			setState(HOVERING);
			return;
		}
		break;

	case STANDING:

		if (sprite->getCurrentAnim() == sprite->SMILE || sprite->getCurrentAnim() == sprite->LANDING) break;

		sprite->play(sprite->STAND);

		// Get sleepy after awhile
		if ((SDL_GetTicks() - timeAwake) / 1000 > 30) {
			setState(SLEEPY);
		}

		// Do a lil stretch after waking up
		if ((SDL_GetTicks() - timeAwake) / 1000 > 1 && (SDL_GetTicks() - timeAwake) / 1000 < 2) {
			sprite->play(sprite->STRETCH, 500, 1000);
			return;
		}

		if (y != floorY) {
			sprite->play(sprite->LIFTOFF, 100, 30);
			setState(HOVERING); 
			return;
		}

		if (x < floorX || x > floorX + floorWidth) {
			setState(FLYING);
			return;
		}

		// Walk to mouse if its far away
		if (mousex < x - 100 && !sprite->isFacingLeft()) sprite->flip();
		if (mousex > x + 100 && sprite->isFacingLeft()) sprite->flip();
		if (abs(mousex - x) > 400) {

			// Target is either somewhere on the floor, or at its edge
			targetX = (mousex > floorX && mousex < floorX + floorWidth) ? mousex :
				(mousex < floorX) ?
				floorX + sprite->getSize() / 2 :
				floorX + floorWidth - sprite->getSize() / 2;
			if (!targetX) targetX = 1;
			setState(WALKING);
		}
		break;

	case HOVERING:

		if (sprite->getCurrentAnim() == sprite->LIFTOFF) break;

		sprite->play(sprite->HOVER, 100);

		// Set target to new floor
		targetY = floorY - 20;
		targetX = floorX + (floorWidth / 2) + landingOffset;
		
		// Fly up
		if (targetX < x) { if (!sprite->isFacingLeft()) sprite->flip(); x--; }
		if (targetX > x) { if (sprite->isFacingLeft()) sprite->flip(); x++; }
		if (y < targetY) y += 3;
		if (y > targetY) y -= 3;

		if (abs(y - targetY) < 5) {
			setState(FLYING);
		}
		break;

	case FLYING:
		timeAwake = SDL_GetTicks() - 3000;
		sprite->play(sprite->FLY, 100);
		
		// Set target to new floor
		targetY = floorY;
		targetX = floorX + (floorWidth / 2) + landingOffset;

		if (y < targetY) y++;
		if (y > targetY) y--;

		if (targetX < x) { if (!sprite->isFacingLeft()) sprite->flip(); x-=6; }
		if (targetX > x) { if (sprite->isFacingLeft()) sprite->flip(); x+=6; }


		if (abs(y - targetY) > 100) {
			setState(HOVERING);
		}

		if ((floorX > areaX || floorX < 0) && flyby) {
			floorY--;
		}

		if (x >= areaX + 30 || x <= -30) {
			x = (x <= 30)? areaX + 20 : - 20;
			y = areaY - 40;
			floorY = areaY;
			floorX = 0;
			floorWidth = areaX;
			flyby = 0;
		}

		if (y == floorY && (x > floorX + 10 && x < floorX + floorWidth - 10)) {
			sprite->play(sprite->LANDING, 200, 200);
			setState(STANDING);
		}
		break;

	}

	
	//Swap transition animations into passive animations after termination
	if(sprite->animationTerminated()) {
		switch (sprite->getCurrentAnim()) {
		case sprite->WAKE:
			setState(STANDING);
			break;
		case sprite->SMILE:
			sprite->play(STANDING);
			setState(STANDING);
			break;
		case sprite->LIFTOFF:
			sprite->play(HOVERING);
			break;
		case sprite->STRETCH:
			setState(STANDING);
			break;
		case sprite->LANDING:
			sprite->play(STANDING);
			setState(STANDING);
			timeAwake = SDL_GetTicks() - 3000;
			break;
		case sprite->TIRED2:
			setState(SLEEPING);
			y += 8;
		}
	}
}