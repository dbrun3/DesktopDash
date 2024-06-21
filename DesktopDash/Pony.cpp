#include "Pony.h"

Pony::Pony(Sprite* s, int aX, int aY) {
	sprite = s;
	areaX = aX;
	areaY = aY;

	x = 3 * areaX / 4;
	y = areaY + 8;

	floorWidth = areaX;

	sprite->play(SLEEP, 400);
	isSleeping = true;
}

Pony::~Pony() {
	//TODO
}

void Pony::pressed() {
	if (isSleeping) {
		sprite->play(WAKE);
		y -= 8;
		isSleeping = false;
		isWaking = true;
	}
}

void Pony::update() {
	
	int tempx, tempy;
	SDL_GetGlobalMouseState(&tempx, &tempy);

	if (!isSleeping) {
		timeAwake++;
	}

	if (targetX) {
		

		if (abs(targetX - x) < 5 || abs(tempx - x) < 5) {
			targetX = 0;
			sprite->play(STAND);
			isStanding = true;
			isWalking = false;
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

	}

	if (sprite->animationTerminated()) {

		if (isWaking) {
			sprite->play(STAND);
			isWaking = false;
			isStanding = true;
		}
	}

	if (isStanding) {
		if (tempx < x) if (!sprite->isFacingLeft()) sprite->flip();
		if (tempx > x) if (sprite->isFacingLeft()) sprite->flip();
		if (abs(tempx - x) > 300) {
			targetX = (tempx < floorX + floorWidth) ? tempx : floorX + floorWidth;
			if (!targetX) targetX = 1;
			sprite->play(WALK);
			isWalking = true;
			isStanding = false;
		}

	}
}