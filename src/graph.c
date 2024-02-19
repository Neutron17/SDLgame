#include "graph.h"
#include "base/log.h"
#include "base/exitCodes.h"
#include "global.h"

SDL_Window *window;
SDL_Renderer *renderer;
int width = 500, height = 500;

extern void distanceInit(int n);
extern Pos *distance;

void graphInit(void) {
	distanceInit(8);
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOGF(L_ERR, "Couldn't init SDL, '%s'", SDL_GetError());
		cleanUp(E_SDL);
	}
	window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
	if(!window) {
		LOGF(L_ERR, "Couldn't create window, '%s'", SDL_GetError());
		cleanUp(E_SDL);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) {
		LOGF(L_ERR, "Couldn't createe renderer, '%s'", SDL_GetError());
		cleanUp(E_SDL);
	}
}
void graphDestroy(void) {
	free(distance);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

SDL_Texture *textureLoad(const char *fname) {
	SDL_Surface *surf = SDL_LoadBMP(fname);
	if(!surf) {
		LOGF(L_ERR, "Couldn't load texture from file '%s', '%s'", fname, SDL_GetError());
		cleanUp(E_SDL);
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	return texture;
}

