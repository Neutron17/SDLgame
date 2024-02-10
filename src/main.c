#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "exitCodes.h"
#include "log.h"
#include "lt.h"

typedef struct {
	int x,y;
	int w,h;
} Entity;


void graphInit(void);
void graphDestroy(void);
void handleInput(void);

#define DELAY (1000/60)

SDL_Window *window;
SDL_Renderer *renderer;
int width = 500, height = 500;
bool quit;
Entity player;
Uint64 deltaTime;

int main(int argc, char *argv[]) {
	init();
	player = (Entity){ width/2, height/2, 50, 50 };
	SDL_Event e;
	Uint64 startT;
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
				handleInput();
				break;
			default: break;
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_Rect playerRect = { player.x, player.y, player.w, player.h };
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &playerRect);

		//SDL_RenderCopy()
		SDL_RenderPresent(renderer);

		deltaTime = SDL_GetTicks64() - startT;
		if(DELAY > deltaTime)
			SDL_Delay(DELAY - deltaTime);
	}
	cleanUp(E_SUCCESS);
}

void handleInput(void) {
	const Uint8 *states = SDL_GetKeyboardState(NULL);
	if(states[SDL_SCANCODE_ESCAPE] || states[SDL_SCANCODE_Q]) {
		quit = true;
		return;
	}
	const int speed = 15;

	if(states[SDL_SCANCODE_W])
		player.y -= speed;
	if(states[SDL_SCANCODE_A])
		player.x -= speed;
	if(states[SDL_SCANCODE_S])
		player.y += speed;
	if(states[SDL_SCANCODE_D])
		player.x += speed;
}

void graphInit(void) {
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

