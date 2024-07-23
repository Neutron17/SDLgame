#include <math.h>
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
#include "movement.h"
#include "moves.h"
#include "tile.h"

#define DELAY (1000/60)

// accessed in input.c
bool quit;
static Uint64 deltaTime;
Entity player;

Pos moveGimp(Entity *self, const Uint8 *keystates, TileProp tile) {
	const double a = atan2(player.y - self->y, player.x - self->x);
	const int speeeed = 2;
	//Pos p = POS(abs(player.x-self->x), 2.0/abs(player.y-self->y));
	Pos p = POS(self->x + speeeed*cos(a), self->y + speeeed*sin(a));
	return p;
}

int main(int argc, char *argv[]) {
	init();

	SDL_Texture *playerTexture = textureLoad("box.bmp");
	//player = entity(width/2, height/2, 50, 50, playerTexture);
	struct EntityState st = (struct EntityState){ .health = 100 };
	player = entity(POS(10, 10), 50, 50, st, playerTexture);
	movementBindCb(&player, movePlayer);

	SDL_Texture *tileTexture = textureLoad("1.bmp");
	SDL_Texture *lava = textureLoad("lava.bmp");

	SDL_Texture *gimpTex = textureLoad("gimp.bmp");
	Entity gimp = _entity(POS(50,50), 75, 75, ET_HAS_HEALTH | ET_ENEMY | ET_COLLIDE, (struct EntityState) { .health=200 }, gimpTex);
	movementBindCb(&gimp, moveGimp);
	
	Uint64 startT;
	SDL_Event e;
	SDL_Texture *textures[] = { tileTexture, lava };
	TileProp props[] = { (TileProp){ 0.5, 0 }, (TileProp) { 2, 10 } };
	TileEnv env = { .textures=textures, .properties=props, .len=1 };
	int tiles[] = { 
		0, 0, 0, 0, 0,
		0, 1, 0, 1, 0,
		0, 1, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	tilemapSet(env, tiles, 5, 4);
	while(!quit) {
		startT = SDL_GetTicks();
		SDL_PollEvent(&e);
		switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					SDL_GetWindowSize(window, &width, &height);
				break;
			default: break;
		}
		movementMoveAll();
		//printf("%d %d\n", player.x, player.y);

		const SDL_Rect playerRect = { 
			width/2 - (player.w/2), height/2 - (player.h/2), 
			player.w, player.h };
		const SDL_Rect gimpRect = { 
			gimp.x - player.x + width/2 - 75/2, gimp.y - player.y + height/2 - 75/2,
			gimp.w, gimp.h };

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		tilemapDraw();
		SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
		SDL_RenderCopy(renderer, gimpTex, NULL, &gimpRect);
		printf("%d\n", player.state.health);

		SDL_RenderPresent(renderer);

		deltaTime = SDL_GetTicks64() - startT;
		if(DELAY > deltaTime)
			SDL_Delay(DELAY - deltaTime);
	}
	entitysystemRemove(&player);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(tileTexture);
	SDL_DestroyTexture(lava);
	SDL_DestroyTexture(gimpTex);
	cleanUp(E_SUCCESS);
}

void init(void) {
	loginit(L_ALL, L_ALL);
	graphInit("Title", 500, 500);
	entitysystemInit();
	movementInit();
}
_Noreturn void cleanUp(int status) {
	movementDestroy();
	entitysystemDestroy();
	logdestroy();
	graphDestroy();
	exit(status);
}

