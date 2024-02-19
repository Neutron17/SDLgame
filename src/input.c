#include "input.h"
#include "global.h"
#include <stdbool.h>

extern bool quit;
static Pos dest;

void input(SDL_Event e) {
	switch (e.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_WINDOWEVENT:
			if(e.window.event == SDL_WINDOWEVENT_RESIZED)
				SDL_GetWindowSize(window, &width, &height);
			break;
		case SDL_KEYDOWN: {// e.key.keysym.scancode
			const Uint8 *states = SDL_GetKeyboardState(NULL);
			if(states[SDL_SCANCODE_ESCAPE] || states[SDL_SCANCODE_Q]) {
				quit = true;
				return;
			}
			const int speed = SPEED - (states[SDL_SCANCODE_LSHIFT] ? SPEED/2 : 0);

			if(states[SDL_SCANCODE_W])
				player.y -= speed;
			if(states[SDL_SCANCODE_A])
				player.x -= speed;
			if(states[SDL_SCANCODE_S])
				player.y += speed;
			if(states[SDL_SCANCODE_D])
				player.x += speed;
		}; break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&dest.x, &dest.y);
			break;
		default: break;
	}
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

