#include <SDL2/SDL_scancode.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "base/exitCodes.h"
#include "base/log.h"
#include "base/lt.h"
#include "entitysystem.h"

void graphInit(void);
void graphDestroy(void);
void distanceInit(int n);
void tilemapDraw(int n, SDL_Texture *tile);
void keyInput(void);
Pos mouseInput(void);
void mouseFollow(Pos dest);
bool edgeCollision(const Uint8 *state);
SDL_Texture *textureLoad(const char *fname);

#define NTH(ARR, N, Y, X) (ARR[N*Y + X])
#define DELAY (1000/60)
#define POS(x, y) ((Pos) { x,y })
#define TILE_SZ (70)
#define SPEED (15)

SDL_Window *window;
SDL_Renderer *renderer;
int width = 500, height = 500;
bool quit;
Entity player;
Uint64 deltaTime;
/** [x,y] 
 * 1x1 example:
 * [[[-1,-1],[0,-1],[1,-1]],[[-1,0],[0,0],[1,0]],[[-1,1],[0,1],[1,1]]] */
Pos *distance;

int main(int argc, char *argv[]) {
	init();
	Uint64 startT;
	Pos dest = {0,0};

	SDL_Texture *texture = textureLoad("box.bmp");
	player = entity(width/2, height/2, 50, 50, texture);

	SDL_Texture *text1 = textureLoad("1.bmp");
	
	SDL_Event e;
	while(!quit) {
		startT = SDL_GetTicks64();
		SDL_PollEvent(&e);
		switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					SDL_GetWindowSize(window, &width, &height);
				break;
			case SDL_KEYDOWN: // e.key.keysym.scancode
				keyInput();
				break;
			case SDL_MOUSEBUTTONDOWN:
				dest = mouseInput();
				break;
			default: break;
		}
		if(0) // TODO
			mouseFollow(dest);

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

void keyInput(void) {
	const Uint8 *states = SDL_GetKeyboardState(NULL);
	if(states[SDL_SCANCODE_ESCAPE] || states[SDL_SCANCODE_Q]) {
		quit = true;
		return;
	}
	//if(edgeCollision(states))
		//return;
	const int speed = SPEED - (states[SDL_SCANCODE_LSHIFT] ? SPEED/2 : 0);

	if(states[SDL_SCANCODE_W])
		player.y -= speed;
	if(states[SDL_SCANCODE_A])
		player.x -= speed;
	if(states[SDL_SCANCODE_S])
		player.y += speed;
	if(states[SDL_SCANCODE_D])
		player.x += speed;
}
Pos mouseInput(void) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	return (Pos){mx,my};
}
void mouseFollow(Pos dest) {
	if(abs(dest.x - player.x) > 4 && abs(dest.y - player.y) > 4) {
		const int dx = dest.x - player.x;
		const int dy = dest.y - player.y;
		const double magn = sqrt(dx*dx + dy*dy); // Pythagoras
		const double vx = dx / magn;
		const double vy = dy / magn;

		player.x += vx * SPEED;
		player.y += vy * SPEED;
	}
}
bool edgeCollision(const Uint8 *states) {
	if(states[SDL_SCANCODE_W] && (player.y - player.h/2 - SPEED) < 0) {
		player.y = player.h/2;
		return true;
	} else if(states[SDL_SCANCODE_A] && (player.x - player.w/2 - SPEED) < 0) {
		player.x = player.w/2;
		return true;
	} else if(states[SDL_SCANCODE_S] && (player.y + player.h/2 + SPEED) > height) {
		player.y = height - player.h/2;
		return true;
	} else if(states[SDL_SCANCODE_D] && (player.x + player.w/2 + SPEED) > width) {
		player.x = width - player.w/2;
		return true;
	}
	return false;
}

void tilemapDraw(int n, SDL_Texture *tile) {
	const Pos playertile = POS(width/2 - (player.x % TILE_SZ), 
				height/2 - (player.y % TILE_SZ));
	SDL_Rect rect = {0,0,TILE_SZ,TILE_SZ};
	for(int i = 0; i < (2*n+1); i++) {
		for(int j = 0; j < (2*n+1); j++) {
			const Pos offs = NTH(distance, (2*n+1), i, j);
			rect.x = playertile.x + offs.x;
			rect.y = playertile.y + offs.y;
			SDL_RenderCopy(renderer, tile, NULL, &rect);
		}
	}
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

void distanceInit(int n) {
	const int dim = (2*n+1)*(2*n+1);
	distance = malloc(sizeof(Pos) * dim);
	if(!distance) {
		LOGF(L_ERR, "Couldn't allocate for distance");
		cleanUp(E_ALLOC);
	}
	for(int i = -n; i <= n; i++)
		for(int j = -n; j <= n; j++)
			distance[(2*n+1)*(i+n) + j+n] = POS(TILE_SZ*j, TILE_SZ*i);
}
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

