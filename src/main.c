#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "exitCodes.h"
#include "log.h"
#include "lt.h"

typedef struct {
	int x,y;
} Pos;

typedef struct {
	int x,y;
	int w,h;
} Entity;

void graphInit(void);
void graphDestroy(void);
void distanceInit(int n);
void tilemapDraw(int n, SDL_Texture *tile); // TODO: naming
void keyInput(void);
Pos mouseInput(void);
bool edgeCollision(const Uint8 *state);

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
Pos *distance; // TODO: naming

int main(int argc, char *argv[]) {
	init();
	player = (Entity){ width/2, height/2, 50, 50 };
	SDL_Event e;
	Uint64 startT;
	Pos dest = {0,0};

	SDL_Surface *surf = SDL_LoadBMP("box.bmp");
	if(!surf) {
		LOGF(L_ERR, "Couldn't load texture, '%s'", SDL_GetError());
		cleanUp(E_SDL);
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_Surface *surf1 = SDL_LoadBMP("1.bmp");
	if(!surf) {
		LOGF(L_ERR, "Couldn't load texture, '%s'", SDL_GetError());
		cleanUp(E_SDL);
	}
	SDL_Texture *text1 = SDL_CreateTextureFromSurface(renderer, surf1);
	

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

		/*if(abs(dest.x - player.x) > 4 && abs(dest.y - player.y) > 4) {
			const int dx = dest.x - player.x;
			const int dy = dest.y - player.y;
			const double magn = sqrt(dx*dx + dy*dy);
			const double vx = dx / magn;
			const double vy = dy / magn;

			player.x += vx * SPEED;
			player.y += vy * SPEED;
		}*/
		SDL_Rect playerRect = { player.x - (player.w/2), player.y - (player.h/2), player.w, player.h };

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		tilemapDraw(8, text1);
		SDL_RenderCopy(renderer, texture, NULL, &playerRect);
		//SDL_RenderFillRect(renderer, &playerRect);

		SDL_RenderPresent(renderer);

		deltaTime = SDL_GetTicks64() - startT;
		if(DELAY > deltaTime)
			SDL_Delay(DELAY - deltaTime);
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(text1);
	SDL_FreeSurface(surf);
	SDL_FreeSurface(surf1);
	cleanUp(E_SUCCESS);
}

void keyInput(void) {
	const Uint8 *states = SDL_GetKeyboardState(NULL);
	if(states[SDL_SCANCODE_ESCAPE] || states[SDL_SCANCODE_Q]) {
		quit = true;
		return;
	}
	if(edgeCollision(states))
		return;

	if(states[SDL_SCANCODE_W])
		player.y -= SPEED;
	if(states[SDL_SCANCODE_A])
		player.x -= SPEED;
	if(states[SDL_SCANCODE_S])
		player.y += SPEED;
	if(states[SDL_SCANCODE_D])
		player.x += SPEED;
}
Pos mouseInput(void) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	return (Pos){mx,my};
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
	const Pos playertile = POS(player.x - (player.x % TILE_SZ), 
				player.y - (player.y % TILE_SZ));
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
}
void cleanUp(int status) {
	logdestroy();
	graphDestroy();
	exit(status);
}

