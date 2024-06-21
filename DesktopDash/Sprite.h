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

	void play(int animation) { current_animation = animation; animation_speed = 100; }
	void play(int animation, int speed) { current_animation = animation; animation_speed = speed; }
	void flip() { facingLeft = !facingLeft; }
	int status() { return errCode; }

	enum errors {
		OK,
		FILE_NOT_FOUND,
		MEM_NOT_ALLOCD,
	};

private:

	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect, destRect;

	Animation* animations = nullptr;

	int current_animation = 0;
	int animation_speed = 100;
	int size = 0;
	int frames = 0;
	bool facingLeft = true;
	int errCode = 0;

};

