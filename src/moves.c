#include "moves.h"
#include "dmg.h"
#include "global.h"
#include "base/log.h"

#include <stdbool.h>

#define SPEED (15)

extern bool quit;
static Pos dest;

PosF movePlayer(Entity *self, const Uint8 *keystates, TileProp tile) {
	PosF pl = *(PosF *)self;
	//printf("(%d %d) -> ", pl.x, pl.y);
	if(keystates[SDL_SCANCODE_ESCAPE] || keystates[SDL_SCANCODE_Q]) {
		quit = true;
		return pl;
	}

	const int speed = (SPEED + (keystates[SDL_SCANCODE_LSHIFT] ? SPEED/2 : 0)) * tile.speed_mul;
	dmgResolve(NULL, self, (Dmg){.type=DMG_TILE, .value=tile.dmg_p_sec});

	if(keystates[SDL_SCANCODE_W])
		pl.y -= speed;
	if(keystates[SDL_SCANCODE_A])
		pl.x -= speed;
	if(keystates[SDL_SCANCODE_S])
		pl.y += speed;
	if(keystates[SDL_SCANCODE_D])
		pl.x += speed;
	if(DEBUG)
		LOGF(L_DEBUG, "Player moved to (%d %d)", pl.x, pl.y);
	return pl;
}

PosF moveGimp(Entity *self, const Uint8 *keystates, TileProp tile) {
	const double a = atan2(player.y - self->y, player.x - self->x);
	const double speeeed = 2.0*tile.speed_mul;
	//Pos p = POS(abs(player.x-self->x), 2.0/abs(player.y-self->y));
	PosF p = POSF(self->x + speeeed*cos(a), self->y + speeeed*sin(a));
	dmgResolve(NULL, self, (Dmg){DMG_TILE, tile.dmg_p_sec});
	if(DEBUG)
		LOGF(L_DEBUG, "Gimp moved to (%f %f)", p.x, p.y);
	return p;
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

