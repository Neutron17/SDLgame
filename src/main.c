#include <SDL2/SDL_render.h>
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
#include "tile.h"
#include "input.h"

#define DELAY (1000/60)

bool quit;
static Uint64 deltaTime;
Entity player;

int main(int argc, char *argv[]) {
	init();

	SDL_Texture *playerTexture = textureLoad("box.bmp");
	//player = entity(width/2, height/2, 50, 50, playerTexture);
	player = entity(0, 0, 50, 50, playerTexture);

	SDL_Texture *tileTexture = textureLoad("1.bmp");
	
	Uint64 startT;
	SDL_Event e;
	SDL_Texture *textures[1] = { tileTexture };
	TileEnv env = { .textures=textures, .len=1 };
	int tiles[] = { 
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
	};
	tilemapSet(env, tiles, 5, 4);
	while(!quit) {
		startT = SDL_GetTicks64();
		SDL_PollEvent(&e);
		input(e);
		printf("%d %d\n", player.x, player.y);

		const SDL_Rect playerRect = { width/2 - (player.w/2), height/2 - (player.h/2), player.w, player.h };

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		tilemapDraw();
		SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

		SDL_RenderPresent(renderer);

		deltaTime = SDL_GetTicks64() - startT;
		if(DELAY > deltaTime)
			SDL_Delay(DELAY - deltaTime);
	}
	entitysystemRemove(&player);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(tileTexture);
	cleanUp(E_SUCCESS);
}

void init(void) {
	loginit(L_ALL, L_ALL);
	graphInit("Title", 500, 500);
	entitysystemInit();
}
_Noreturn void cleanUp(int status) {
	entitysystemDestroy();
	logdestroy();
	graphDestroy();
	exit(status);
}

