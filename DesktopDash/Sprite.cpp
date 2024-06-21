#include "Sprite.h"

Sprite::Sprite(const char* filename, int r, int c, int f, int s, SDL_Renderer* renderer) {
	
	texture = TextureManager::LoadTexture(filename, renderer);
	if (!texture) { errCode = FILE_NOT_FOUND; return; }

	animations = (Animation*) malloc(sizeof(Animation) * r * c);
	if (!animations) { errCode = MEM_NOT_ALLOCD; return; }

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			animations[i * c + j] = Animation(i, j * f);
		}
	}

	this->renderer = renderer;
	size = s;
	frames = f;
	destRect.x = destRect.y = srcRect.x = srcRect.y = 0;
	destRect.w = destRect.h = srcRect.w = srcRect.h = s;

}

void Sprite::draw() {

	int row = animations[current_animation].row;
	int col = animations[current_animation].col;

	srcRect.x = srcRect.w * static_cast<int>((((SDL_GetTicks() - (Uint32)animation_start) / animation_speed) % frames) + col);
	srcRect.y = row * srcRect.h;
	if (SDL_GetTicks() - (Uint32) animation_start > animation_speed) animation_terminated = true;

	SDL_RendererFlip flip = facingLeft? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
	TextureManager::Draw(texture, srcRect, destRect, flip, renderer);
}