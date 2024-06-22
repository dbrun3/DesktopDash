#pragma once

#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"

class Sprite
{
public:

	//							  rows  cols  frames  size
	Sprite(const char* filename, int r, int c, int f, int s, SDL_Renderer* render);
	~Sprite();

	void draw();

	void play(int animation) { 
		if (animation == current_animation) return;
		current_animation = animation; animation_speed = 100; animation_terminated = false; animation_start = SDL_GetTicks(); }
	void play(int animation, int speed) { 
		if (animation == current_animation) return;
		current_animation = animation; animation_speed = speed; animation_terminated = false; animation_start = SDL_GetTicks(); 
	}
	void flip() { facingLeft = !facingLeft; }
	bool isFacingLeft() { return facingLeft; }
	bool animationTerminated() { return animation_terminated; }
	int getSize() { return size; }
	int status() { return errCode; }

	enum errors {
		OK,
		FILE_NOT_FOUND,
		MEM_NOT_ALLOCD,
	};

	enum animation_names {
		STANCE1,
		KICK, //useless (?)
		WALK,
		STAND,
		LANDING,
		STRETCH, //useless (?)
		HOVER,
		POINT, //useless
		CRY,
		STANCE2,
		FLY,
		BLINK,
		CONFUSED,
		TIRED1, //useless
		TIRED2,
		LIFTOFF,
		WAKE,
		SLEEP
	};

private:

	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect, destRect;

	Animation* animations = nullptr;

	int current_animation = 0;
	int animation_speed = 100;
	int animation_terminated = false;
	double animation_start = 0;
	int size = 0;
	int frames = 0;
	bool facingLeft = true;
	int errCode = 0;

};

