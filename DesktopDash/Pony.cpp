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
	floorX = 0;
	floorY = areaY;
	floorWidth = areaX;
}

void Pony::window_mode(int wx, int wy, int width) {
	floorX = wx;
	floorY = wy;
	floorWidth = width;
}

void Pony::pressed() {
	if (state[SLEEPING] || state[SLEEPY]) {
		sprite->play(sprite->WAKE);
		timeAwake = SDL_GetTicks();
		if (state[SLEEPING]) y -= 8;
		setState(WAKING);

	}
}

void Pony::update() {
	
	int mousex, mousey;
	SDL_GetGlobalMouseState(&mousex, &mousey);

	switch (getState()) {

	case WAKING:
		if (sprite->animationTerminated()) {
			sprite->play(sprite->STAND);
			setState(STANDING);
		}
		break;

	case SLEEPY:
		if ((SDL_GetTicks() - timeAwake) / 1000 > 50) {
			sprite->play(sprite->SLEEP, 800);
			setState(SLEEPING);
			y += 8;
		}
		break;

	case WALKING:
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
		break;

	case STANDING:
		// Get sleepy after awhile
		if ((SDL_GetTicks() - timeAwake) / 1000 > 40) {
			sprite->play(sprite->TIRED2, 1000);
			setState(SLEEPY);
		}

		// Do a lil stretch after waking up
		if ((SDL_GetTicks() - timeAwake) / 1000 > 1 && (SDL_GetTicks() - timeAwake) / 1000 < 2 && !state[WAKING]) {
			sprite->play(sprite->STRETCH, 1000);
			setState(WAKING);
			return;
		}

		//Follow the mouse
		if (mousex < x - 100) if (!sprite->isFacingLeft()) sprite->flip();
		if (mousex > x + 100) if (sprite->isFacingLeft()) sprite->flip();
		if (abs(mousex - x) > 400) {

			// Target is either somewhere on the floor, or at its edge
			targetX = (mousex > floorX && mousex < floorX + floorWidth) ? mousex :
				(mousex < floorX) ?
				floorX + sprite->getSize() / 2 :
				floorX + floorWidth - sprite->getSize() / 2;
			if (!targetX) targetX = 1;
			sprite->play(sprite->WALK);
			setState(WALKING);
		}
		break;

	}



	
}