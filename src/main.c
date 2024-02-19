#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "base/exitCodes.h"
#include "base/log.h"
#include "base/lt.h"

#include "global.h"
#include "graph.h"
#include "entitysystem.h"
#include "tilemap.h"
#include "input.h"

#define DELAY (1000/60)

bool quit;
static Uint64 deltaTime;
Entity player;

int main(int argc, char *argv[]) {
	init();

	SDL_Texture *texture = textureLoad("box.bmp");
	player = entity(width/2, height/2, 50, 50, texture);

	SDL_Texture *text1 = textureLoad("1.bmp");
	
	Uint64 startT;
	SDL_Event e;
	while(!quit) {
		startT = SDL_GetTicks64();
		SDL_PollEvent(&e);
		input(e);

		SDL_Rect playerRect = { width/2 - (player.w/2), height/2 - (player.h/2), player.w, player.h };

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		tilemapDraw(8, text1);
		SDL_RenderCopy(renderer, texture, NULL, &playerRect);

		SDL_RenderPresent(renderer);

		deltaTime = SDL_GetTicks64() - startT;
		if(DELAY > deltaTime)
			SDL_Delay(DELAY - deltaTime);
	}
	entitysystemRemove(&player);
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(text1);
	cleanUp(E_SUCCESS);
}

void init(void) {
	loginit(L_ALL, L_ALL);
	graphInit();
	entitysystemInit();
}
_Noreturn void cleanUp(int status) {
	entitysystemDestroy();
	logdestroy();
	graphDestroy();
	exit(status);
}

