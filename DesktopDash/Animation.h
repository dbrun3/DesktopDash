#pragma once

struct Animation {
	int row = 0;
	int col = 0;

	Animation() {}

	Animation(int r, int c) {
		row = r; // what row of the spritesheet to use
		col = c; // what col to start from
	}
};