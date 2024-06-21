#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filename, SDL_Renderer* renderer) {

	SDL_Surface* tempSurface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, SDL_Renderer* renderer) {
	SDL_RenderCopyEx(renderer, texture, &src, &dest, NULL, NULL, flip);
}